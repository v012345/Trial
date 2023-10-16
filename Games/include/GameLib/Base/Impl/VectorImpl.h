#ifndef INCLUDED_GAMELIB_VECTORIMPL_H
#define INCLUDED_GAMELIB_VECTORIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/Vector.h"

namespace GameLib{

template< class T > inline Vector< T >::Vector( int size, int capacity ) : 
mElements( 0 ),
mSize( size ),
mCapacity( capacity ){
	if ( capacity <= 0 ){
		mCapacity = mSize;
	}
	if ( mSize > 0 ){
		mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		for ( int i = 0; i < mSize; ++i ){
			new ( &mElements[ i ] ) T;
		}
	}
}

template< class T > inline Vector< T >::Vector( const Vector& a ) : 
mElements( 0 ),
mSize( a.mSize ),
mCapacity( a.mCapacity ){
	if ( mSize > 0 ){
		mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		for ( int i = 0; i < mSize; ++i ){
			new ( &mElements[ i ] ) T( a.mElements[ i ] ); //调用复制构造函数
		}
	}
}

template< class T > inline Vector< T >::~Vector(){
	clear();
}

template< class T > inline void Vector< T >::operator=( const Vector& a ){ 
	//编写operator =（）时，请注意赋值给自己的情况。
	if ( this == &a ){
		return;
	}
	//首先，将其完全删除。即使有足够大的区域，也会删除它，因为可能会浪费掉它。
	clear();

	mSize = a.size;
	mCapacity = a.mCapacity;
	if ( mSize > 0 ){
		mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		for ( int i = 0; i < mSize; ++i ){
			new ( &mElements[ i ] ) T( a.mElements[ i ] ); //调用复制构造函数
		}
	}
}


template< class T > inline void Vector< T >::push( const T& a ){
	if ( mSize == mCapacity ){ //没有地方 执行移动。
		//1.5确保双倍+1大小的区域。+1是为了即使不满2也能增加。
		mCapacity = ( mCapacity * 3 / 2 ) + 1;
		T* newElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		//丢弃所有副本和旧元素
		for ( int i = 0; i < mSize; ++i ){
			new ( &newElements[ i ] ) T( mElements[ i ] );
			mElements[ i ].~T();
		}
		//销毁旧空间
		OPERATOR_DELETE( mElements );
		//
		mElements = newElements;
	}
	//添加新元素
	new ( &mElements[ mSize ] ) T( a );
	//更新大小
	++mSize;
}

template< class T > inline T* Vector< T >::push(){
	if ( mSize == mCapacity ){ //没有地方 执行移动。
		//1.5确保双倍+1大小的区域。+1是为了即使不满2也能增加。
		mCapacity = ( mCapacity * 3 / 2 ) + 1;
		T* newElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		//丢弃所有副本和旧元素
		for ( int i = 0; i < mSize; ++i ){
			new ( &newElements[ i ] ) T( mElements[ i ] );
			mElements[ i ].~T();
		}
		//销毁旧空间
		OPERATOR_DELETE( mElements );
		//
		mElements = newElements;
		newElements = 0;
	}
	//添加新元素
	new ( &mElements[ mSize ] ) T;
	//更新大小
	++mSize;
	//返回指向新元素的指针
	return &mElements[ mSize - 1 ];
}

template< class T > inline void Vector< T >::clear(){
	//析构函数调用
	for ( int i = 0; i < mSize; ++i ){
		mElements[ i ].~T();
	}
	//释放空间
	OPERATOR_DELETE( mElements );
	mSize = mCapacity = 0;
}

template< class T > inline int Vector< T >::size() const {
	return mSize;
}

template< class T > inline void Vector< T >::setSize( int n ){
	//
	if ( n < mCapacity ){
		for ( int i = n; i < mSize; ++i ){ //必须向后释放。如果有5和3，则需要释放下标3和4
			mElements[ i ].~T();
		}
	}else if ( n > mCapacity ){ //不行 
		mCapacity = n;
		T* newElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		//丢弃所有副本和旧元素
		for ( int i = 0; i < mSize; ++i ){
			new ( &newElements[ i ] ) T( mElements[ i ] );
			mElements[ i ].~T();
		}
		//销毁旧空间
		OPERATOR_DELETE( mElements );
		//
		mElements = newElements;
		newElements = 0;
		//调用默认构造函数
		for ( int i = mSize; i < n; ++i ){
			new ( &mElements[ i ] ) T;
		}
	}
	mSize = n;
}

template< class T > inline T& Vector< T >::operator[]( int i ){
	ASSERT( i >= 0 && i < mSize && "Range Error!" );
	return mElements[ i ];
}

template< class T > inline const T& Vector< T >::operator[]( int i ) const {
	ASSERT( i >= 0 && i < mSize && "Range Error!" );
	return mElements[ i ];
}

template< class T > inline void Vector< T >::moveTo( Vector* a ){
	a->clear();
	a->mElelents = mElements;
	a->mSize = mSize;
	a->mCapacity = mCapacity;
	mElements = 0;
	mSize = mCapacity = 0;
}

} //namespace GameLib

#endif
