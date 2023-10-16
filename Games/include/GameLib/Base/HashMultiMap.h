#ifndef INCLUDED_GAMELIB_HASHMULTIMAP_H
#define INCLUDED_GAMELIB_HASHMULTIMAP_H

#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Hash.h"
#include "GameLib/Base/Array.h"

namespace GameLib{

///指定容量上限的哈希表

template< class Key, class Value, class H = Hash< Key > > class HashMultiMap{
public:
	///默认。必须调用setCapacity
	HashMultiMap();
	///指定最大容量。表的大小默认为大于capacity的最小素数。
	explicit HashMultiMap( int capacity, int tableSize = 0 );
	~HashMultiMap();
	///再次确保。在有内容的状态下调用将触发assert。执行clear()
	void setCapacity( int capacity, int tableSize = 0 );
	///获取最大容量
	int capacity() const;
	///获得当前元素数
	int size() const;
	///全部释放
	void clear();
	///添加。返回追加的位置。
	int add( const Key&, const Value& );
	///添加。返回追加的位置。在getValue后再写入值
	int add( const Key& );
	///查找。返回的是迭代器。同一个键的开头。
	int find( const Key& ) const;
	///指定并删除迭代器。删除后返回真。
	bool remove( int position );
	///键获取（禁止重新赋值，仅const）
	const Key* key( int position ) const;
	///获取值（const）
	const Value* value( int position ) const;
	///获取值（非const）
	Value* value( int position );
	//
	int next( int position ) const;
	int first() const;
	//判断position是否为结尾。
	bool isEnd( int position ) const;
	///转换成简单的数组。根据size()的尺寸申请的空间
	void copyTo( Value* ) const;
	//复制到Arry。允许传递空
	void copyTo( Array< Value >* ) const;
private:
	void operator=( const HashMultiMap& ); //禁止赋值
	HashMultiMap( const HashMultiMap& ); //禁止复制构造函数

	int* mNexts;
	Key* mKeys;
	Value* mValues;
	Stack< int > mEmptyStack;
	int mCapacity;
	int mSize;
	int mTableSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/HashMultiMapImpl.h" //里面是这个

#endif

