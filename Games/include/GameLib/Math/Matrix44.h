#ifndef INCLUDED_GAMELIB_MATH_MATRIX44_H
#define INCLUDED_GAMELIB_MATH_MATRIX44_H

namespace GameLib{
namespace Math{

class Vector3;
class Vector4;
class Matrix34;


class Matrix44{
public:
	Matrix44();
	void setIdentity();
	//生成透视变换矩阵
	void setPerspectiveTransform( 
		float fieldOfViewYInRadian,
		float windowWidth,
		float windowHeight,
		float nearClip,
		float farClip,
		bool zBufferIsLinear = false );
	//生成正交矩阵
	void setOrthogonalTransform(
		float left,
		float right,
		float bottom,
		float top,
		float front,
		float back );
	//创建视图矩阵并即时进行乘法
	void multiplyViewTransform(
		const Vector3& eyePosition,
		const Vector3& eyeTarget,
		const Vector3& upVector );

	void mul( Vector4* out, const Vector3& in ) const;
	void operator*=( const Matrix34& );
	void setMul( const Matrix44&, const Matrix34& );
	///检查是否完全一致
	bool operator==( const Matrix44& ) const;
	///检查一下有没有不同
	bool operator!=( const Matrix44& ) const;

	float& operator()( int row, int col );
	float operator()( int row, int col ) const;

	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
};

} //namespace Math
} //namespace GameLib

#endif
