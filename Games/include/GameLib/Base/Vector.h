#ifndef INCLUDED_GAMELIB_VECTOR_H
#define INCLUDED_GAMELIB_VECTOR_H

namespace GameLib{

///简易版std：vector

template< class T > class Vector{
public:
	//指定capacity后，到达该值后不会迁移
	explicit Vector( int size = 0, int capacity = 0 );
	///整个拷贝。速度会比较慢
	Vector( const Vector& ); //不要禁止复制构造函数
	~Vector();
	//代入。复制整个。性能会比较差
	void operator=( const Vector& ); //不禁止赋值
	///添加
	void push( const T& );
	///追加元素。返回的指针表示它存储的位置。
	T* push();
	///删除最后的元素
	void pop();
	///全部删除
	void clear();
	///获取大小
	int size() const;
	///调整大小。即使变小也不会迁移。
	void setSize( int );
	///下标索引（非const）
	T& operator[]( int );
	///下标索引（const）
	const T& operator[]( int ) const;
	///把里面的东西转移到别的vector。自己清空，旧数据也会消失
	void moveTo( Vector* );
private:
	T* mElements;
	int mSize;
	int mCapacity;
};

} //namespace GameLib
#include "GameLib/Base/Impl/VectorImpl.h" //里面是这个

#endif

