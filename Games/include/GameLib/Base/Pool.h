#ifndef INCLUDED_GAMELIB_POOL_H
#define INCLUDED_GAMELIB_POOL_H

namespace GameLib{

///某一类型的放置场所。主要为了减少new的使用。

template< class T > class Pool{
public:
	explicit Pool( int defaultBlockSize = 16 );
	~Pool();
	///修改默认块数。
	void setDefaultBlockSize( int blockSize );
	///全部释放
	void clear();
	//确保n个
	T* allocate( int n = 1 );
	//n个拷贝构造
	T* allocate( const T*, int n = 1 );
private:
	struct Block{
		Block* mNext; //下一块
		T* mElements; //数组
		int mUsedNumber; //使用数
		int mSize; //块大小
	};
	void operator=( const Pool& ); //禁止赋值
	Pool( const Pool& ); //禁止复制构造函数

	//常量
	int mDefaultBlockSize; //单位量
	Block mHead; //dummy节点。
	Block* mLastBlock; //最后一个节点
};

} //namespace GameLib
#include "GameLib/Base/Impl/PoolImpl.h" //里面是这个

#endif
