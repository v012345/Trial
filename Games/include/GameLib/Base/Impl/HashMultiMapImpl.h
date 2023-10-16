#ifndef INCLUDED_GAMELIB_HASH_MULTIMAPIMPL_H
#define INCLUDED_GAMELIB_HASH_MULTIMAPIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/HashMultiMap.h"
#include "GameLib/Base/PrimeNumber.h"



namespace GameLib{

//要做一些工作，以使开头和结尾不需要特别处理。
template< class K, class V, class H > inline HashMultiMap< K, V, H >::HashMultiMap() : 
mNexts( 0 ),
mKeys( 0 ),
mValues( 0 ),
mCapacity( 0 ),
mSize( 0 ),
mTableSize( 0 ){
	ASSERT( sizeof( K ) != 0 ); //有可能吗
}

//要做一些工作，以使开头和结尾不需要特别处理。
template< class K, class V, class H > inline HashMultiMap< K, V, H >::HashMultiMap( int capacity, int tableSize ) : 
mNexts( 0 ),
mKeys( 0 ),
mValues( 0 ),
mCapacity( 0 ),
mSize( 0 ),
mTableSize( 0 ){
	ASSERT( sizeof( K ) != 0 ); //有可能吗
	setCapacity( capacity, tableSize );
}

template< class K, class V, class H > inline HashMultiMap< K, V, H >::~HashMultiMap(){
	if ( mNexts ){
		int pos = mNexts[ mCapacity ];
		while ( pos < mCapacity + mTableSize ){ //如果有节点
			if ( pos < mCapacity ){
				mKeys[ pos ].~K();
				mValues[ pos ].~V();
			}
			pos = mNexts[ pos ];
		}
		OPERATOR_DELETE( mKeys ); //直接delete
		OPERATOR_DELETE( mValues ); //直接delete
		SAFE_DELETE_ARRAY( mNexts );
	}
}

template< class K, class V, class H > inline void HashMultiMap< K, V, H >::setCapacity( int capacity, int tableSize ){
	ASSERT( mSize == 0 && "NOT EMPTY! call clear()." );
	//先清除
	if ( mCapacity > 0 ){
		mEmptyStack.clear();
		clear();
		OPERATOR_DELETE( mValues ); //直接delete
		OPERATOR_DELETE( mKeys ); //直接delete
		SAFE_DELETE_ARRAY( mNexts );
	}
	//重新分配
	mCapacity = capacity;
	if ( capacity <= 0 ){ //忽略0
		return;
	}	
	if ( tableSize == 0 ){ //自动设置大于capacity的质数
		tableSize = capacity;
	}
	tableSize = PrimeNumber::next( tableSize );
	if ( tableSize < 3 ){
		tableSize = 3; //至少3
	}
	mTableSize = tableSize;
	//
	mNexts = NEW int[ mCapacity + mTableSize ];
	//分配值数组。
	mKeys = static_cast< K* >( OPERATOR_NEW( sizeof( K ) * mCapacity ) );
	mValues = static_cast< V* >( OPERATOR_NEW( sizeof( V ) * mCapacity ) );
	//存储空的编号
	mEmptyStack.setCapacity( mCapacity );
	for ( int i = 0; i < mCapacity; ++i ){
		mEmptyStack.push( i ); //放入空编号列表
	}
	//从mCapacity + 0到mCapacity + mTab​​leSize-1的数字是伪元素。位于每列开头之前。
	for ( int i = 0; i < mTableSize; ++i ){
		mNexts[ mCapacity + i ] = mCapacity + i + 1;
	}
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::capacity() const {
	return mCapacity;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::add( const K& k, const V& v ){
	ASSERT( mSize < mCapacity );
	//哈希函数计算
	int h = H().value( k, mTableSize );
	//查找
	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mKeys[ p ], k ) ){
			break;
		}
		p = mNexts[ p ];
	}
	int newPos;
	mEmptyStack.pop( &newPos );
	new ( &mKeys[ newPos ] ) K( k ); //调用复制构造函数
	new ( &mValues[ newPos ] ) V( v ); //调用复制构造函数
	if ( p >= mCapacity ){ //如果没有，请添加到顶部
		//插入表格顶部
		int head = mCapacity + h;
		mNexts[ newPos ] = mNexts[ head ]; //
		mNexts[ head ] = newPos; //将先前的链接连接到新节点
	}else{ //有时在之后添加
		mNexts[ newPos ] = mNexts[ p ];
		mNexts[ p ] = newPos; //将先前的链接连接到新节点
	}
	++mSize;
	return newPos;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::add( const K& k ){
	ASSERT( mSize < mCapacity );
	//哈希函数计算
	int h = H().value( k, mTableSize );
	//查找
	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mKeys[ p ], k ) ){
			break;
		}
		p = mNexts[ p ];
	}
	int newPos;
	mEmptyStack.pop( &newPos );
	new ( &mKeys[ newPos ] ) K( k ); //调用复制构造函数
	new ( &mValues[ newPos ] ) V;
	if ( p >= mCapacity ){ //如果没有，请添加到顶部
		//插入表格顶部
		int head = mCapacity + h;
		mNexts[ newPos ] = mNexts[ head ]; //
		mNexts[ head ] = newPos; //将先前的链接连接到新节点
	}else{ //有时在之后添加
		mNexts[ newPos ] = mNexts[ p ];
		mNexts[ p ] = newPos; //将先前的链接连接到新节点
	}
	++mSize;
	return newPos;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::find( const K& k ) const{
	int h = H().value( k, mTableSize );

	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mKeys[ p ], k ) ){
			return p;
		}
		p = mNexts[ p ];
	}
	return mCapacity + mTableSize;
}

template< class K, class V, class H > inline bool HashMultiMap< K, V, H >::remove( int position ){
	//
	//因此，步进迭代器，直到它出现在表中。
	int h = position;
	while ( h < mCapacity ){
		h = mNexts[ h ];
	}
	//一旦超出范围。从中减去mCapacity可获得下一个哈希值。
	h -= mCapacity;
	//-1的话这就是哈希值了。
	h -= 1;
	//现在我们知道了哈希值，我们通过从表顶部进行跟踪来找到p。
	int prev = mCapacity + h; //头
	int p = mNexts[ prev ];
	while ( p < mCapacity ){
		if ( p == position ){ //发现。销毁它
			mNexts[ prev ] = mNexts[ p ]; //
			mKeys[ p ].~K(); //析构
			mValues[ p ].~V(); //析构
			mEmptyStack.push( p ); //放入空栈
			--mSize;
			return true;
		}
		prev = p;
		p = mNexts[ p ];
	}
	return false;
}

template< class K, class V, class H > inline const V* HashMultiMap< K, V, H >::value( int p ) const {
	return &mValues[ p ];
}

template< class K, class V, class H > inline V* HashMultiMap< K, V, H >::value( int p ){
	return &mValues[ p ];
}

template< class K, class V, class H > inline const K* HashMultiMap< K, V, H >::key( int p ) const {
	return &mKeys[ p ];
}

template< class K, class V, class H > inline void HashMultiMap< K, V, H >::clear(){
	//析构
	int prev = mCapacity; //头
	int p = mNexts[ mCapacity ]; //dummy头
	while ( p < mCapacity + mTableSize ){  //mCapacity + mTab​​leSize不存在。到目前为止，它作为虚拟对象存在。
		if ( p < mCapacity ){
			mNexts[ prev ] = mNexts[ p ]; //
			mKeys[ p ].~K();
			mValues[ p ].~V();
			mEmptyStack.push( p ); //放入空栈
		}
		prev = p;
		p = mNexts[ p ];
	}
	mSize = 0;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::next( int p ) const {
	ASSERT( p >= 0 );
	while ( p < mCapacity + mTableSize ){
		p = mNexts[ p ];
		if ( p < mCapacity ){ //返回非虚拟节点
			return p;
		}
	}
	return p;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::first() const {
	int p = mCapacity; //第一个dummy
	while ( p < mCapacity + mTableSize ){
		p = mNexts[ p ];
		if ( p < mCapacity ){ //返回非虚拟节点
			return p;
		}
	}
	return mCapacity + mTableSize;
}

template< class K, class V, class H > inline bool HashMultiMap< K, V, H >::isEnd( int position ) const {
	return ( position == mCapacity + mTableSize );
}

template< class K, class V, class H > inline void HashMultiMap< K, V, H >::copyTo( V* a ) const {
	int p = mNexts[ mCapacity ];
	int idx = 0;
	while ( p != mCapacity + mTableSize ){
		if ( p < mCapacity ){
			a[ idx ] = mValues[ p ];
			++idx;
		}
		p = mNexts[ p ];
	}
}


template< class K, class V, class H > inline void HashMultiMap< K, V, H >::copyTo( Array< V >* a ) const {
	a->setSize( mSize );
	if ( mSize > 0 ){
		copyTo( &( ( *a )[ 0 ] ) );
	}
}
		


template< class K, class V, class H > inline int HashMultiMap< K, V, H >::size() const {
	return mSize;
}

} //namespace GameLib

#endif
