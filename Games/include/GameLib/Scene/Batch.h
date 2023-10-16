#ifndef INCLUDED_GAMELIB_SCENE_BATCH_H
#define INCLUDED_GAMELIB_SCENE_BATCH_H

namespace GameLib{

namespace Math{
	class Vector3;
};

namespace Scene{

using namespace Math;
class Batch;

///绘制接口

class Batch{
public:
	///绘制
	void draw() const;
	///绘制（带修改颜色）
	void draw( const Vector3& color, float transparency ) const;

	Batch();
	~Batch();
	Batch( const Batch& );
	operator void*() const;
	Batch& operator=( const Batch& );
	void release();
	bool operator==( const Batch& ) const;
	bool operator!=( const Batch& ) const;

	class Impl;
	explicit Batch( Impl* );
private:
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
