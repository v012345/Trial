#include "GameLib/GameLib.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Scene/Font.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Scene/PrimitiveRenderer.h"
#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Array.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include <algorithm>
#include <string>
using namespace std;

namespace GameLib{
namespace Scene{

using namespace GameLib::Math;
using namespace GameLib::Graphics;

//用于排序的类
template< class T > struct PointerLess{
	bool operator()( const T* a, const T* b ){
		return ( *a < *b );
	}
};

class StringRenderer::Impl : public ReferenceType{
public:
	Impl( int charCapacity, int requestCapacity ) :
	mCharCapacity( charCapacity ),
	mRequests( requestCapacity ){ //
		//TODO：状态更改指令的最大数量太多了
		mPrimitiveRenderer = PrimitiveRenderer::create( charCapacity * 2, charCapacity + 4 );
		//调用两次draw
		mPreviousFrameId = Manager().frameId() - 1; //通过的值
	}
	~Impl(){
	}
	void add( const Vector2& p, const char* s, unsigned c, bool wrap, float depth ){
		STRONG_ASSERT( mCurrentFont && "Font hasn't been set." );
		mRequests.push(); //
		Request& r = *mRequests.get();
		r.mPosition = p;
		r.mDepth = depth;
		r.mString = s;
		r.mColor = c;
		r.mFont = mCurrentFont;
		r.mWrap = wrap;
	}
	void draw(){
		//帧ID检查
		unsigned fid = Manager().frameId();
		STRONG_ASSERT( mPreviousFrameId != fid && "StringRenderer::draw() : you can't draw() twice in a frame!" );
		mPreviousFrameId = fid;

		if ( mRequests.size() == 0 ){ //从不做
			return;
		}
		//将标准状态发布到TriangleRenderer
		mPrimitiveRenderer.setBlendMode( Graphics::BLEND_LINEAR );
		mPrimitiveRenderer.setCullMode( Graphics::CULL_NONE );
		mPrimitiveRenderer.enableDepthTest( false );
		mPrimitiveRenderer.enableDepthWrite( false );
		//获取当前视口并浮动
		Vector2 screenSize;
		int tw, th;
		Graphics::Manager().getViewport( 0, 0, &tw, &th );
		screenSize.set( 
			static_cast< float >( tw ),
			static_cast< float >( th ) );

		//准备字符缓冲区
		Array< Char > chars( mCharCapacity );
		int charPosition = 0;
		//获取请求并处理。
		while ( mRequests.size() > 0 ){
			Request& r = *mRequests.get();
			Vector2 pos = r.mPosition;
			float charHeight = static_cast< float >( r.mFont.charHeight() );
			//一次取出一个字符。必须是unsigned char
			int n = static_cast< int >( r.mString.size() );
			const unsigned char* s = reinterpret_cast< const unsigned char* >( r.mString.c_str() );
			for ( int i = 0; i < n; ++i ){
				//准备char
				int code = s[ i ];
				//ShiftJis范围？
				if ( ( code >= 0x81 && code <= 0x9F ) || ( code >= 0xE0 && code <= 0xFC ) ){
					if ( ( i + 1 < n ) ){
						code <<= 8;
						code |= s[ i + 1 ]; //2字节代码
						++i;
					}
				}
				if ( code == '\n' ){ //这是换行符。
					pos.x = r.mPosition.x;
					pos.y += charHeight;
					continue;
				}
				//获得文字
				Char& c = chars[ charPosition ];
				bool found = r.mFont.getChar( 
					&c.mTexture,
					&c.mUv0,
					&c.mUv1,
					code );
				if ( found ){ //有字符
					//将像素添加到当前位置。
					c.mSize.setSub( c.mUv1, c.mUv0 );
					//给定纹理的宽度和高度，算出要转换为像素的比率。
					c.mSize.x *= static_cast< float >( c.mTexture.width() );
					c.mSize.y *= static_cast< float >( c.mTexture.height() );
				}else{ //
					c.mSize.set( charHeight, charHeight ); //正方形
				}
				//剔除计算
				//
				if ( r.mWrap ){
					if ( pos.x + c.mSize.x > screenSize.x ){
						pos.x = r.mPosition.x;
						pos.y += charHeight;
					}
				}else{ //
					if ( pos.x > screenSize.x ){
						break;
					}
				}
				//仅当纵向完全溢出时才结束。
				if ( pos.y > screenSize.y ){
					break;
				}
				//添加条目
				c.mColor = r.mColor;
				c.mPosition.set( pos.x, pos.y, r.mDepth );
				++charPosition;
				//发送字符
				pos.x += c.mSize.x;
			}
			mRequests.pop();
		}
		if ( charPosition == 0 ){ //没有字符
			return;
		}
		//处理后，按Z和纹理对缓冲区进行排序
		Array< Char* > charPointers( charPosition );
		for ( int i = 0; i < charPosition; ++i ){
			charPointers[ i ] = &chars[ i ];
		}
		sort( &charPointers[ 0 ], &charPointers[ 0 ] + charPosition, PointerLess< Char >() );
		//按排序顺序将字符缓冲区添加到TriangleBuffer
		Texture lastTexture;
		for ( int i = 0; i < charPosition; ++i ){
			Char& c = *charPointers[ i ];
			//设置纹理是否与之前不同
			if ( lastTexture != c.mTexture ){
				mPrimitiveRenderer.setTexture( c.mTexture );
				lastTexture = c.mTexture;
			}
			//创建顶点并绘制一个矩形
			Vector2 p[ 2 ];
			p[ 0 ].set( c.mPosition.x, c.mPosition.y ); //左上点
			p[ 1 ].set( c.mPosition.x + c.mSize.x, c.mPosition.y + c.mSize.y ); //右下
			//绘制
			if ( c.mTexture ){ //如果纹理
				mPrimitiveRenderer.addRectangle( 
					p[ 0 ], p[ 1 ],
					c.mUv0, c.mUv1,
					c.mColor );
			}else{ //
				mPrimitiveRenderer.addRectangle( 
					p[ 0 ], p[ 1 ],
					c.mColor );
			}
		}
		//绘制TriangleRenderer，因为一切都结束了
		mPrimitiveRenderer.draw();
	}
	void setFont( Font& font ){
		mCurrentFont = font;
	}
private:
	struct Request{
		Vector2 mPosition;
		float mDepth;
		string mString;
		unsigned mColor;
		Font mFont;
		bool mWrap;
	};
	//字符结构体
	struct Char{
		//与Z和纹理进行比较。Z是背面
		bool operator<( const Char& a ) const {
			if ( mPosition.z > a.mPosition.z ){
				return true;
			}else if ( mPosition.z < a.mPosition.z ){
				return false;
			}else if ( mTexture < a.mTexture ){
				return true;
			}else{
				return false;
			}
		}
		Vector3 mPosition; //左上点
		Vector2 mUv0; //左上UV
		Vector2 mUv1; //右下UV
		Vector2 mSize;
		Texture mTexture; //纹理
		unsigned mColor;
	};
	int mCharCapacity;
	Stack< Request > mRequests;
	PrimitiveRenderer mPrimitiveRenderer;
	Font mCurrentFont;
	unsigned mPreviousFrameId;
};

StringRenderer StringRenderer::create( int charCapacity, int requestCapacity ){
	StringRenderer r;
	r.mImpl = NEW Impl( charCapacity, requestCapacity );
	return r;
}

void StringRenderer::add( int x, int y, const char* s, unsigned c, bool wrap, float z ){
	Vector2 p( 
		static_cast< float >( x ),
		static_cast< float >( y ) );
	mImpl->add( p, s, c, wrap, z );
}

void StringRenderer::add( const Vector2& p, const char* s, unsigned c, bool wrap, float z ){
	mImpl->add( p, s, c, wrap, z );
}

void StringRenderer::draw(){
	mImpl->draw();
}

void StringRenderer::setFont( Font& font ){
	mImpl->setFont( font );
}

#define TYPE StringRenderer
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

} //namespace Scene
} //namespace GameLib
