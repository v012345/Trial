#ifndef INCLUDED_STRING_RENDERER_H
#define INCLUDED_STRING_RENDERER_H

class Image;

//字符串绘制类
//它也是单例模式的示例。
class StringRenderer{
public:
	static StringRenderer* instance();
	//绘图函数。默认白
	void draw( int x, int y, const char* string, unsigned color = 0xffffff ) const;
	static void create( const char* fontFileName ); //调用并初始化
	static void destroy(); //调用则结束处理
private:
	StringRenderer( const char* fontFileName ); //设为私有。
	StringRenderer( StringRenderer& ); //屏蔽副本构造函数。不要在cpp中编写。
	~StringRenderer(); //设为私有。

	Image* mImage;
	static StringRenderer* mInstance; //唯一的实例
};

#endif