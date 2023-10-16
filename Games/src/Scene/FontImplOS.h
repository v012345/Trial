#ifndef INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H
#define INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H

#include "Scene/FontImpl.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include <map> //想成自己写的...

#include <windows.h>
#undef min
#undef max

namespace GameLib{
namespace Scene{
using namespace std;

using namespace GameLib::Graphics;
using namespace GameLib::Math;

namespace { //匿名

//从Windows获取字体数据
class FontImplOS : public Font::Impl{
	struct Char;
public:
	FontImplOS(
	const char* fontName,
	int charHeightHint, //并非总是如此
	bool bold,
	bool italic,
	bool proportional,
	bool antialias,
	int textureWidth,
	int textureHeight ) :
	mCharHeight( 0 ),
	mCharAscent( 0 ),
	mAntiAliased( antialias ),
	mFontHandle( 0 ),
	mDeviceContextHandle( 0 ),
	mCurrentX( 0 ),
	mCurrentY( 0 ),
	mCurrentLineHeight( 0 ),
	mCurrentPage( 0 ),
	mTextureWidth( textureWidth ),
	mTextureHeight( textureHeight ){
		//纹理生成。
		Texture t = Texture::create( mTextureWidth, mTextureHeight, false );
		//储存在map中
		mTextures.insert( make_pair( 0, t ) );

		//将字体名称转换为UNICODE
		DWORD pitch = ( proportional ) ? VARIABLE_PITCH : FIXED_PITCH;

		//字体生成
		mFontHandle = CreateFontA(
			charHeightHint,
			0,
			0,
			0,
			( bold ) ? FW_BOLD : FW_DONTCARE,
			( italic ) ? TRUE : FALSE,
			FALSE,
			FALSE,
			SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY,
			pitch | FF_DONTCARE,
			fontName );
		ASSERT( mFontHandle );
		//生成HDC
		mDeviceContextHandle = CreateCompatibleDC( NULL );
		//获取文本
		//字体集
		SelectObject( mDeviceContextHandle, mFontHandle );
		//
		TEXTMETRIC tm;
		GetTextMetrics( mDeviceContextHandle, &tm );
		mCharHeight = tm.tmHeight; //用实际值覆盖。
		mCharAscent = tm.tmAscent; //需要尺寸超出基线
	}
	~FontImplOS(){
		DeleteDC( mDeviceContextHandle );
		mDeviceContextHandle = 0;
	
		DeleteObject( mFontHandle );
		mFontHandle = 0;
	}
	bool getChar(
	Texture* texture,
	Vector2* uvTopLeft,
	Vector2* uvBottomRight,
	int code ){
		//
		CharIt cit = mChars.find( static_cast< unsigned short >( code ) );
		const Char* c = 0;
		if ( cit != mChars.end() ){
			c = &cit->second;
		}else{
			c = createGlyph( code );
		}
		TextureIt tit = mTextures.find( c->mTexturePage );
		ASSERT( tit != mTextures.end() );
		*texture = tit->second;
		float tw = static_cast< float >( texture->width() );
		float th = static_cast< float >( texture->height() );
		float w = static_cast< float >( c->mWidth );
		float h = static_cast< float >( c->mHeight );
		float x = static_cast< float >( c->mX );
		float y = static_cast< float >( c->mY );
		uvTopLeft->set( x / tw, y / th );
		uvBottomRight->set( 
			( x + w ) / tw, 
			( y + h ) / th );
		return true;
	}
	const Char* createGlyph( int code ){
		//字体集
		SelectObject( mDeviceContextHandle, mFontHandle );

		GLYPHMETRICS gm; // 信息
		MAT2 mat = {
			{ 0, 1 }, 
			{ 0, 0 }, 
			{ 0, 0 }, 
			{ 0, 1 }
		};
		DWORD size;
		const UINT format = ( mAntiAliased ) ? GGO_GRAY8_BITMAP : GGO_BITMAP;
		// 首先获取所需缓冲区大小
		size = GetGlyphOutlineA(
			mDeviceContextHandle,
			static_cast< UINT >( code ),
			format,
			&gm, 0, NULL, &mat );
		//尺寸获取
		int cellW = static_cast< int >( gm.gmCellIncX );
		int cellH = static_cast< int >( mCharHeight ); //IncY没用。
		int glyphW = static_cast< int >( gm.gmBlackBoxX );
		int glyphH = static_cast< int >( gm.gmBlackBoxY );
		int glyphX = static_cast< int >( gm.gmptGlyphOrigin.x );
		int glyphY = static_cast< int >( mCharAscent - gm.gmptGlyphOrigin.y );
		if ( glyphW + glyphX > cellW ){ //别无选择
			cellW = glyphW + glyphX;
		}
		if ( glyphH + glyphY > cellH ){ //别无选择
			cellH = glyphH + glyphY;
		}
		//获得写入纹理
		TextureIt tit = mTextures.find( mCurrentPage );
		ASSERT( tit != mTextures.end() );
		Texture* tex = &( tit->second );
		int tw = tex->width();
		int th = tex->height();
		//溢出！
		if ( mCurrentX + cellW >= tw ){
			mCurrentX = 0;
			mCurrentY += mCurrentLineHeight;
			mCurrentLineHeight = 0;
			//纵向溢出！
			if ( mCurrentY + cellH >= th ){
				mCurrentY = 0;
				++mCurrentPage;
				Texture t = Texture::create( mTextureWidth, mTextureHeight, false );
				//储存在map中
				mTextures.insert( make_pair( mCurrentPage, t ) );
				//重新纹理
				TextureIt tit = mTextures.find( mCurrentPage );
				ASSERT( tit != mTextures.end() );
				tex = &( tit->second );
			}
		}
		//纹理锁（部分锁）
		unsigned* dst;
		int dstPitch;
		tex->lock( 
			&dst, 
			&dstPitch,
			mCurrentX,
			mCurrentY,
			cellW,
			cellH );

		if ( size == 0 ){ //只需填0即可
			for ( int y = 0; y < cellH; ++y ){
				for ( int x = 0; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
			}
		}else{
			// 确保缓冲
			BYTE* data = NEW BYTE[ size ];
			int srcPitch = ( glyphW + 3 ) & ~( 3 ); //4字节参数

			// 第二次调用才是核心。获取数据。
			size = GetGlyphOutlineA(
				mDeviceContextHandle,
				static_cast< UINT >( code ),
				format,
				&gm, size, data, &mat );
			ASSERT( size != GDI_ERROR );

			//1比特时太不方便，复制到字节排列中
			if ( mAntiAliased ){ //如果应用了anti，则执行64-> 255灰度转换
				for ( DWORD i = 0; i < size; ++i ){
					data[ i ] = data[ i ] * 255 / 64;
				}
			}else{
				BYTE* newData = NEW BYTE[ size * 8 ]; //8倍就够了
				const BYTE* src = data;
				BYTE* dst = newData;
				int srcPitch = ( ( ( glyphW + 7 ) / 8 ) + 3 ) & ( ~3 );
				int dstPitch = ( glyphW + 3 ) & ( ~3 );
				for ( int y = 0; y < glyphH; ++y ){
					for ( int x = 0; x < glyphW; ++x ){
						int shift = 7 - ( x % 8 );
						int mask = 0x1 << shift;
						int bit = ( src[ x / 8 ] & mask ) >> shift;
						dst[ x ] = static_cast< BYTE >( bit * 255 );
					}
					dst += dstPitch;
					src += srcPitch;
				}
				//删除旧数组
				SAFE_DELETE_ARRAY( data );
				//用新数组替换
				data = newData;
			}
			BYTE* src = data;
			//顶部空白填充
			int y = 0;
			for ( ; y < glyphY; ++y ){
				for ( int x = 0; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
			}
			//
			for ( ; y < glyphY + glyphH; ++y ){
				LONG x = 0;
				//左侧留空
				for ( ; x < glyphX; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				//
				for ( ; x < glyphX + glyphW; ++x ){
					int a = src[ x - glyphX ];
					dst[ x ] = ( a << 24 ) | 0xffffff;
				}
				//右空白
				for ( ; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
				src += srcPitch;
			}
			//底部空白填充
			for ( ; y < cellH; ++y ){
				for ( int x = 0; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
			}
			SAFE_DELETE_ARRAY( data );
		}
		tex->unlock( &dst, 0 );
		//加到列表
		Char c;
		c.mTexturePage = static_cast< unsigned char >( mCurrentPage );
		c.mWidth = static_cast< unsigned char >( cellW );
		c.mHeight = static_cast< unsigned char >( cellH );
		c.mX = static_cast< unsigned short >( mCurrentX );
		c.mY = static_cast< unsigned short >( mCurrentY );
		mCurrentX += cellW; //往前移动写位置
		if ( mCurrentLineHeight < cellH ){
			mCurrentLineHeight = cellH;
		}

		CharIt it = mChars.insert( make_pair( code, c ) ).first;
		return &( it->second );
	}
	int charHeight() const {
		return mCharHeight;
	}
private:
	void operator=( const FontImplOS& ); //禁止

	struct Char{
		unsigned char mTexturePage; //纹理页
		unsigned char mHeight; //高度信息（像素）
		unsigned char mWidth; //宽度信息（像素）
		unsigned short mX; //左端
		unsigned short mY; //
	};
	typedef map< unsigned short, Char > CharMap;
	typedef CharMap::iterator CharIt;
	typedef map< int, Texture > TextureMap;
	typedef TextureMap::iterator TextureIt;

	int mCharHeight;
	int mCharAscent; //从TEXTMETRIX获取
	bool mAntiAliased;
	HFONT mFontHandle;
	HDC mDeviceContextHandle;
	CharMap mChars;
	TextureMap mTextures;
	int mCurrentX; //下一个字符的左上角X
	int mCurrentY; //下一个字符的左上Y
	int mCurrentLineHeight; //当前行的最大高度
	int mCurrentPage; //当前正在写入的纹理编号
	const int mTextureWidth;
	const int mTextureHeight;
};

} //namespace {}

} //Scene:
} //GameLib
#endif
