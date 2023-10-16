#ifndef INCLUDED_GAMELIB_COLLISIONMESH_H
#define INCLUDED_GAMELIB_COLLISIONMESH_H

namespace GameLib{
namespace PseudoXml{
	class ConstElement;
}
namespace Math{
	class Vector3;
};
namespace Scene{
using namespace Math;

///冲突判定用三角形集合类

class CollisionMesh{
public:
	///从伪XML文件构造
	static CollisionMesh create( const char* filename );
	///根据伪XML的元素（Container）构造
	static CollisionMesh create( PseudoXml::ConstElement );
	///检测载入结束。如果是从文件名中创建的，将会返回true
	bool isReady();
	///线段相交判定。
	bool isIntersect( const Vector3& lineOrigin, const Vector3& lineVector ) const;
	///线段相交判定。第一次碰撞的时间放在timeOut里。如果碰撞了返回true
	bool getIntersection( 
		float* timeOut, //空指针则不返回
		Vector3* normalOut, //空指针则不返回
		const Vector3& lineOrigin, 
		const Vector3& lineVector ) const;

	CollisionMesh();
	~CollisionMesh();
	CollisionMesh( const CollisionMesh& );
	operator void*() const;
	CollisionMesh& operator=( const CollisionMesh& );
	void release();
	bool operator==( const CollisionMesh& ) const;
	bool operator!=( const CollisionMesh& ) const;
private:
	class Impl;
	explicit CollisionMesh( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
