#include "PseudoXml/ElementImpl.h"
#include "PseudoXml/AttributeImpl.h"

#include "GameLib/PseudoXml/Document.h"

namespace GameLib{
namespace PseudoXml{

Document::Impl::Impl() : 
mStringPool( 1024 ), //暂时按1KB
mElementPool( 16 ), //16每个人
mAttributePool( 16 ), //16每个人
mRoot( 0 ){
	mRoot = allocateElement( 1 );
	mRoot->setDocument( this );
}

Document::Impl::~Impl(){
}

bool Document::Impl::isReady(){
	bool r = false;
	if ( mRoot->name() ){ //如果名称不为NULL，则说明已build。
		r = true;
	}else if ( mFile ){ //载入中
		if ( mFile.isFinished() ){ //完成加载
			build( mFile.data(), mFile.size() );
			mFile.release();
			r = true;
		}
	}else{
		HALT( "IT MUST BE BUG!" ); //没有根元素
	}
	return r;
}

void Document::Impl::build( const char* data, int size ){
	if ( RefString( data, 4 ) == "PXML" ){ //二进制！
		IBinaryStream is( data, size );
		is.setPosition( 4 );
		//字符串放入pool
		int stringBufferOffset = is.readInt();
		int stringBufferSize = size - stringBufferOffset;
		is.setPosition( stringBufferOffset );
		char* stringBuffer = mStringPool.allocate( stringBufferSize );
		is.read( stringBuffer, stringBufferSize );
		is.setPosition( 8 );
		mRoot->setName( "" );
		mRoot->setChildNumber( is.readInt() );
		for ( int i = 0; i < mRoot->childNumber(); ++i ){
			mRoot->child( i )->build( &is, stringBuffer );
		}
	}else{ //文字内容
		const char* begin = data;
		const char* end = begin + size;
		Tag tag( "<ROOT>" );
		mRoot->build( &tag, &begin, end );
	}
}

//只需一一调用
void Document::Impl::convertToString( string* out ) const {
	OStringStream oss;
	int childN = mRoot->childNumber();
	for ( int i = 0; i < childN; ++i ){
		const Element::Impl* e = mRoot->child( i );
		if ( e->name() ){ //
			e->convertToString( &oss, 0 );
		}
	}
	Array< char > t;
	oss.get( &t );
	*out = &t[ 0 ];
}

//只需一一调用
void Document::Impl::convertToBinary( Array< char >* out ) const {
	int childN = mRoot->childNumber();
	OStringStream binaryOut;
	OStringStream stringBuffer;
	map< RefString, int > stringMap;
	binaryOut.write( "PXML", 4  ); //magic
	binaryOut.write( 0 ); //字符串缓冲区偏移
	binaryOut.write( childN ); //元素数
	for ( int i = 0; i < childN; ++i ){
		const Element::Impl* e = mRoot->child( i );
		if ( e->name() ){ //
			e->convertToBinary( &binaryOut, &stringBuffer, &stringMap );
		}
	}
	int stringBufferOffset = binaryOut.size();
	//合并字符串缓冲区
	stringBuffer.get( out );
	binaryOut.write( &( *out )[ 0 ], out->size() );
	out->clear();

	binaryOut.get( out );
	//插入实际的字符串缓冲区偏移量
	const char* p = reinterpret_cast< const char* >( &stringBufferOffset );
	for ( int i = 0; i < 4; ++i ){
		( *out )[ 4 + i ] = p[ i ];
	}
}

Element::Impl* Document::Impl::root(){
	return mRoot;
}

Element::Impl* Document::Impl::allocateElement( int n ){
	return mElementPool.allocate( n );
}

Attribute::Impl* Document::Impl::allocateAttribute( int n ){
	return mAttributePool.allocate( n );
}

} //namespace PseudoXml
} //namespace GameLib

