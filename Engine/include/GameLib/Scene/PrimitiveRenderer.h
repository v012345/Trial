#ifndef INCLUDED_GAMELIB_SCENE_PRIMITIVERENDERER_H
#define INCLUDED_GAMELIB_SCENE_PRIMITIVERENDERER_H

#include "GameLib/Graphics/Enum.h"

namespace GameLib{

namespace Graphics{
	class Texture;
}

namespace Math{
	class Matrix44;
	class Vector4;
	class Vector3;
	class Vector2;
}
using namespace Math;

namespace Scene{

///方便直接描绘三角形、线段、点和2D矩形的类。性能会差一些。
class PrimitiveRenderer{
public:
	///指定顶点数和其他命令数然后构建
	static PrimitiveRenderer create( 
		int vertexCapacity, 
		int commandCapacity );

	void setTexture( Graphics::Texture );
	void enableDepthTest( bool );
	void enableDepthWrite( bool );
	void setBlendMode( Graphics::BlendMode );
	void setCullMode( Graphics::CullMode );
	void setTransform( const Matrix44& );

	///用转换好的顶点来绘制三角形。也就是说p是含有4个元素的向量。
	void addTransformedTriangle(
		const Vector4& pos0,
		const Vector4& pos1,
		const Vector4& pos2,
		const Vector2& uv0,
		const Vector2& uv1,
		const Vector2& uv2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff);
	///用转换好的顶点来绘制三角形。无纹理。
	void addTransformedTriangle(
		const Vector4& pos0,
		const Vector4& pos1,
		const Vector4& pos2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff);
	///画三角形。
	void addTriangle(
		const Vector3& pos0,
		const Vector3& pos1,
		const Vector3& pos2,
		const Vector2& uv0,
		const Vector2& uv1,
		const Vector2& uv2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff );
	///画三角形。无纹理
	void addTriangle(
		const Vector3& pos0,
		const Vector3& pos1,
		const Vector3& pos2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff );
	///画线段
	void addLine(
		const Vector3& pos0,
		const Vector3& pos1,
		const Vector2& uv0,
		const Vector2& uv1,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff );
	///画线段。无纹理。
	void addLine(
		const Vector3& pos0,
		const Vector3& pos1,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff );
	///画点。
	void addPoint(
		const Vector3& pos,
		const Vector2& uv,
		unsigned color = 0xffffffff );
	///画点。无纹理
	void addPoint(
		const Vector3& pos,
		unsigned color = 0xffffffff );
	//在Z=0的平面画矩形。坐标系是屏幕坐标系。
	void addRectangle(
		const Vector2& pos0,
		const Vector2& pos1,
		const Vector2& uv0,
		const Vector2& uv1,
		unsigned color = 0xffffffff,
		float depth = 0.f );
	//在Z=0的平面画矩形。坐标系是屏幕坐标系。无纹理。
	void addRectangle(
		const Vector2& pos0,
		const Vector2& pos1,
		unsigned color = 0xffffffff,
		float depth = 0.f );
	///绘制
	void draw();

	//以下用户不会意识到
	PrimitiveRenderer();
	PrimitiveRenderer( const PrimitiveRenderer& );
	~PrimitiveRenderer();
	operator void*() const;
	PrimitiveRenderer& operator=( const PrimitiveRenderer& );
	void release();
	bool operator==( const PrimitiveRenderer& ) const;
	bool operator!=( const PrimitiveRenderer& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Scene

} //namespace GameLib


#endif
