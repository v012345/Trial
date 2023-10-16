#ifndef INCLUDED_GAMELIB_GRAPHICS_MANAGER_H
#define INCLUDED_GAMELIB_GRAPHICS_MANAGER_H

#include "GameLib/Graphics/Enum.h"

namespace GameLib{

namespace Math{
	class Matrix44;
	class Matrix34;
	class Vector2;
	class Vector3;
}
using namespace Math;
	
namespace Graphics{

class Texture;
class VertexBuffer;
class IndexBuffer;

class Manager{
public:
	Manager();
	static Manager instance();

	void setVertexBuffer( VertexBuffer );
	void setIndexBuffer( IndexBuffer );
	void setTexture( Texture );

	void enableDepthTest( bool );
	void enableDepthWrite( bool );
	void setBlendMode( BlendMode );
	void setCullMode( CullMode );
	void setTextureFilter( TextureFilter );
	///透视变换视图转换矩阵。
	void setProjectionViewMatrix( const Matrix44& );
	///世界变换矩阵
	void setWorldMatrix( const Matrix34& );
	///viewport设置
	void setViewport( int x, int y, int width, int height );
	///获取viewport
	void getViewport( int* xOut, int* yOut, int* widthOut, int* heightOut );

	///根据设置的纹理、顶点缓冲和索引缓冲来绘制
	void drawIndexed(
		int offset, 
		int primitiveNumber, 
		PrimitiveType = PRIMITIVE_TRIANGLE );
	///根据设置的纹理和顶点缓冲来绘制
	void draw( 
		int offset, 
		int primitiveNumber, 
		PrimitiveType = PRIMITIVE_TRIANGLE );
	///纹理贴到全屏幕
	void blendToScreen( Texture );
	///全屏化
	void enableFullScreen( bool );
	///返回绘图缓冲区的宽度
	int width() const;
	///返回绘图缓冲区的高度
	int height() const;
	///返回全屏窗口宽度
	int fullScreenWindowWidth() const;
	///返回全屏窗口的高度
	int fullScreenWindowHeight() const;
	///返回帧的编号
	unsigned frameId() const;
	///截图（自动判断扩展名DDS、TGA）。保存时间在绘图结束后。
	void captureScreen( const char* filename );

	//光照关系
	///启用阴影
	void setLightingMode( LightingMode );
	///光源位置
	void setLightPosition( int index, const Vector3& position );
	///光强度（100的话表示100米处光照为1的亮度。作为强度I，亮度由“I/距离”决定）
	void setLightIntensity( int index, float intensity );
	///光源色
	void setLightColor( int index, const Vector3& );
	///视点位置
	void setEyePosition( const Vector3& position );
	///漫反射光色
	void setDiffuseColor( const Vector3& );
	///高光色
	void setSpecularColor( const Vector3& );
	///发光色。
	void setEmissionColor( const Vector3& );
	///环境光色
	void setAmbientColor( const Vector3& );
	///透明度设置
	void setTransparency( float );
	///高光锐度（值越大越尖锐。具体含义请参考其他相关书籍）
	void setSpecularSharpness( float );

	//你不需要知道的东西
	static void create( 
		void* windowHandle,
		int width,
		int height,
		bool fullScreen,
		bool vSync,
		bool antiAlias );
	static void destroy();
	void beginDraw();
	void endDraw();
	bool canRender() const; //如果设备丢失，则返回false。
	void restore(); //
	void getPointerModifier( float* scale, Vector2* offset ) const;
};

} //namespace Graphics
} //namespace GameLib

#endif
