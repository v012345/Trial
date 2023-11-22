#ifndef INCLUDED_MATRIX23_H
#define INCLUDED_MATRIX23_H

class Vector2;

class Matrix23{
public:
	Matrix23(); //默认构造
	//移动矩阵生成
	void setTranslation( const Vector2& );
	//旋转矩阵生成
	void setRotation( double radian );
	//生成缩放矩阵
	void setScaling( const Vector2& );
	//向量与乘法
	void multiply( Vector2* out, const Vector2& in ) const;
	//矩阵与乘法
	void operator*=( const Matrix23& );
	//创建一个移动矩阵并从右侧相乘
	void translate( const Vector2& );
	//
	void rotate( double radian );
	//创建一个缩放矩阵并从右侧相乘
	void scale( const Vector2& );
private:
	double m00, m01, m02;
	double m10, m11, m12;
};

#endif
