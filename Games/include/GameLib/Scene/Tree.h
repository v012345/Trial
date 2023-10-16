#ifndef INCLUDED_GAMELIB_SCENE_TREE_H
#define INCLUDED_GAMELIB_SCENE_TREE_H

namespace GameLib{

namespace Math{
	class Matrix44;
	class Vector3;
	class Vector4;
};

namespace Scene{
class Animation;
using namespace Math;

///层次结构模型
class Tree{
public:
	///绘制
	void draw() const;
	///动画设置
	void setAnimation( Animation );
	///动画更新（参数是时间。负数的话在里面随意进行）
	void updateAnimation( float time = -1.f );
	///动画速度设定
	void setAnimationSpeed( float );

	const Vector3* position() const;
	const Vector3* angle() const; //单位是度
	const Vector3* scale() const;
	const Vector3* color() const;
	float transparency() const;
	void setPosition( const Vector3& );
	void setAngle( const Vector3& ); //单位是度
	void setScale( const Vector3& );
	void setColor( const Vector3& );
	void setTransparency( float );

	Tree();
	~Tree();
	Tree( const Tree& );
	operator void*() const;
	Tree& operator=( const Tree& );
	void release();
	bool operator==( const Tree& ) const;
	bool operator!=( const Tree& ) const;

	class Impl;
	explicit Tree( Impl* );
private:
	Impl* mImpl;
};

//const版本
class ConstTree{
public:
	void draw() const;
	const Vector3* position() const;
	const Vector3* angle() const;
	const Vector3* scale() const;
	const Vector3* color() const;
	float transparency() const;

	//你不需要知道
	ConstTree();
	ConstTree( const Tree& );
	~ConstTree();
	operator void*() const;
	ConstTree& operator=( const Tree& );
	void release();
	bool operator==( const Tree& ) const;
	bool operator!=( const Tree& ) const;
private:
	Tree mBase;
};


} //namespace Scene
} //namespace GameLib

#endif
