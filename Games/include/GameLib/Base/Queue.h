#ifndef INCLUDED_GAMELIB_QUEUE_H
#define INCLUDED_GAMELIB_QUEUE_H

namespace GameLib{

///队列。如果元素个数不超过界限的话就会循环，所以可以一直使用。
template< class T > class Queue{
public:
	///默认。大小，最大容量为0。
	Queue();
	///指定最大容量。
	explicit Queue( int capacity );
	~Queue();
	///再次确保。在有内容的状态下调用将触发assert。执行clear()
	void setCapacity( int size );
	///获取最大容量
	int capacity() const;
	///获得当前元素数
	int size() const;
	///全部释放
	void clear();
	///把空添加到结尾处。因为要返回新增元素的指针，所以初期设定使用这个
	T* pushBack();
	///添加到末尾
	void pushBack( const T& );
	///删除开头
	void popFront();
	///删除开头（获取要丢弃的副本）
	void popFront( T* out );
	///获取开头（const）
	const T* front() const;
	///获取开头（非const）
	T* front();
private:
	void operator=( const Queue& ); //禁止赋值
	Queue( const Queue& ); //禁止复制构造函数

	T* mElements;
	int mSize;
	int mCapacity;
	int mFront;
};

} //namespace GameLib
#include "GameLib/Base/Impl/QueueImpl.h" //里面是这个

#endif

