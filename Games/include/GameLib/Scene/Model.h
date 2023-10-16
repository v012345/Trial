#ifndef INCLUDED_GAMELIB_SCENE_MODEL_H
#define INCLUDED_GAMELIB_SCENE_MODEL_H

namespace GameLib{

namespace Math{
	class Vector3;
	class Vector4;
};

namespace Scene{

using namespace Math;
class Batch;

///绘制实例

class Model{
public:
	///绘制
	void draw() const;

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

	Model();
	~Model();
	Model( const Model& );
	operator void*() const;
	Model& operator=( const Model& );
	void release();
	bool operator==( const Model& ) const;
	bool operator!=( const Model& ) const;

	class Impl;
	explicit Model( Impl* );
private:
	Impl* mImpl;
};

//const版本
class ConstModel{
public:
	void draw() const;
	const Vector3* position() const;
	const Vector3* angle() const;
	const Vector3* scale() const;
	const Vector3* color() const;
	float transparency() const;

	//你不需要知道
	ConstModel();
	ConstModel( const Model& );
	~ConstModel();
	operator void*() const;
	ConstModel& operator=( const Model& );
	void release();
	bool operator==( const Model& ) const;
	bool operator!=( const Model& ) const;
private:
	Model mBase;
};

} //namespace Scene
} //namespace GameLib

#endif
