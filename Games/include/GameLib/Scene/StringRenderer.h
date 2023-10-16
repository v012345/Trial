#ifndef INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H
#define INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H

namespace GameLib{

namespace Graphics{
	class Texture;
}
namespace Math{
	class Vector2;
}
using namespace Math;

namespace Scene{

class Font;

///字符串绘图类。可以指定优先度。自动排序。
class StringRenderer{
public:
	///指定最大字符和最大请求发行数。超越的话
	static StringRenderer create( int charCapacity, int requestCapacity );
	///用像素坐标指定。注意仅z为flat[0,1]
	void add( 
		int x, 
		int y, 
		const char* string, 
		unsigned color = 0xffffffff, 
		bool wrapAround = false,
		float z = 0.f );
	///像素但用float指定位置（x=[0，w]，y=[0，h]，z=[0，1]）
	void add( 
		const Vector2& position, 
		const char* string, 
		unsigned color = 0xffffffff, 
		bool wrapAroung = false, 
		float z = 0.f );
	///一次性把所积攒的请求绘制出来
	void draw();
	///设置字体。至少执行一次才能正常输出文字。
	void setFont( Font& );

	//不需要知道
	StringRenderer();
	~StringRenderer();
	StringRenderer( const StringRenderer& );
	operator void*() const;
	StringRenderer& operator=( const StringRenderer& );
	void release();
	bool operator==( const StringRenderer& ) const;
	bool operator!=( const StringRenderer& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
