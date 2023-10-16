#ifndef INCLUDED_GAMELIB_SCENE_FONT_H
#define INCLUDED_GAMELIB_SCENE_FONT_H

namespace GameLib{

namespace Graphics{
	class Texture;
}
namespace Math{
	class Vector2;
}
using namespace Math;

namespace Scene{

///字体类
class Font{
public:
	///位图字体的每一个字符所需的数据
	struct BitmapChar{
		unsigned short mCode; //字符代码（SHIFT_JIS）
		unsigned short mX; //左端
		unsigned short mY; //
		unsigned char mTexturePage; //纹理页
		unsigned char mWidth; //宽度信息（像素）
		unsigned char mHeight; //高度信息（像素）
	};
	///使用纹理生成位图字体。文字的高度采用最大值。
	static Font create( 
		const char** textureFileNames, //纹理文件名称数组
		int textureNumber,
		BitmapChar* BitmapChars,
		int bitmapCharNumber );
	///使用纹理生成位图字体。文字的高度采用最大值。
	static Font create(
		const char** textureFileData, //纹理DDS图像数组
		const int* textureFileSizes, //纹理数据大小数组
		int textureNumber,
		BitmapChar* bitmapChars,
		int CharNumber );
	//从操作系统中动态获取字体数据。指定字符的高度、内部纹理宽度等。
	static Font create(
		const char* osFontName,
		int charHeightHint, //这是一个参考。实际可以通过getCharHeight获取
		bool bold = false,
		bool italic = false,
		bool proportional = false,
		bool antialias = false,
		int internalTextureWidth = 512,
		int internalTextureHeight = 512 );
	///获取文本信息。如果找不到会返回false
	bool getChar( 
		Graphics::Texture* textureOut,
		Vector2* uvTopLeftOut,
		Vector2* uvBottomRightOut,
		int code );
	///最大字符高度
	int charHeight() const;
	///载入结束了吗？
	bool isReady();

	//以下用户无需关注
	Font();
	~Font();
	Font( const Font& );
	operator void*() const;
	Font& operator=( const Font& );
	void release();
	bool operator==( const Font& ) const;
	bool operator!=( const Font& ) const;

	class Impl;
private:
	explicit Font( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
