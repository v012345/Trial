#ifndef INCLUDED_GAMELIB_COLLISIONDETECTOR_H
#define INCLUDED_GAMELIB_COLLISIONDETECTOR_H

namespace GameLib{
namespace Math{
	class Vector3;
};
template< class T > class Array;
namespace Scene{
using namespace Math;

///球碰撞判定类

class CollisionDetector{
public:
	///结果编号pair
	struct Pair{
		int mId0, mId1;
	};
	static CollisionDetector create( int capacity, int maxLevel = -1 );
	///添加项目。只有detect一次有效。返回值将存入结果ID。
	int add( const Vector3& sphereCenter, float sphereRadius );
	///检测函数。已经add的会被清除。参数会被放入结果ID pair数组
	void detect( Array< Pair >* resultsOut );

	//无需关心的函数
	CollisionDetector();
	~CollisionDetector();
	CollisionDetector( const CollisionDetector& );
	operator void*() const;
	CollisionDetector& operator=( const CollisionDetector& );
	void release();
	bool operator==( const CollisionDetector& ) const;
	bool operator!=( const CollisionDetector& ) const;
private:
	class Impl;
	explicit CollisionDetector( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
