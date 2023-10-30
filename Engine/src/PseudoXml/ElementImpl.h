#ifndef INCLUDED_GAMELIB_PSEUDOXML_ELEMENTIMPL_H
#define INCLUDED_GAMELIB_PSEUDOXML_ELEMENTIMPL_H

#include "GameLib/Base/Vector.h"
#include "GameLib/Base/OStringStream.h"
#include "GameLib/Base/IBinaryStream.h"
#include "PseudoXml/DocumentImpl.h"
#include "PseudoXml/Tag.h"
#include <map>
using namespace std;

namespace GameLib{
namespace PseudoXml{

class Element::Impl{
public:
	Impl() : 
	mChildren( 0 ),
	mChildNumber( 0 ),
	mAttributes( 0 ),
	mAttributeNumber( 0 ),
	mName( 0 ),
	mDocument( 0 ){
	}
	~Impl(){
		//在这里什么也不做，因为所有Document都有实体
		mChildren = 0;
		mAttributes = 0;
		mName = 0;
		mDocument = 0;
	}
	void build( Tag* tag, const char** p, const char* e ){
		//本质上是构造函数，所以第二次调用是非法的
		ASSERT( mAttributeNumber == 0 && mChildNumber == 0 );
		//移植名称

		mName = mDocument->allocateString( *tag->name() );

		//属性移植
		mAttributeNumber = tag->attributeNumber();
		mAttributes = mDocument->allocateAttribute( mAttributeNumber );
		tag->giveAttributes( mAttributes );

		//如果它是一个空元素标签，则没有子元素，因此在此处退出。
		if ( tag->type() == Tag::TYPE_BEGIN_END ){
			return;
		}

		//临时子元素列表
		Tank< Element::Impl > children;
		//查找标签
		while ( *p < e ){
			if ( **p == '<' ){ //发现
				++( *p ); //移至<下一个
				Tag tmpTag( p, e, mDocument ); //解释标签
				Tag::Type type = tmpTag.type();
				if ( type == Tag::TYPE_END ){ //如果结束标记则结束。退出
					break; //完结
				}else if ( type == Tag::TYPE_COMMENT ){
					; //什么都不做
				}else{ //开始元素和空元素都需要此处理
					Element::Impl* tmpE = children.add(); //
					tmpE->setDocument( mDocument );
					tmpE->build( &tmpTag, p, e );
					tmpE = 0;
				}
			}else{
				++( *p ); //忽略直到<出现
			}
		}
		//分配子元素
		mChildNumber = children.size();
		mChildren = mDocument->allocateElement( mChildNumber );
		children.copyTo( mChildren );
	}
	void build( IBinaryStream* s, const char* stringBuffer ){
		mName = stringBuffer + s->readInt();
		mAttributeNumber = s->readInt();
		mAttributes = mDocument->allocateAttribute( mAttributeNumber );
		for ( int i = 0; i < mAttributeNumber ; ++i){
			const char* name = stringBuffer + s->readInt();
			const char* value = stringBuffer + s->readInt();
			mAttributes[ i ].setDocument( mDocument );
			mAttributes[ i ].setNameReference( name );
			if ( *value != '\0' ){
				mAttributes[ i ].setValueReference( value );
			}
		}
		mChildNumber = s->readInt();
		mChildren = mDocument->allocateElement( mChildNumber );
		for ( int i = 0; i < mChildNumber; ++i ){
			mChildren[ i ].setDocument( mDocument );
			mChildren[ i ].build( s, stringBuffer );
		}
	}
	//只需一一调用
	void convertToString( OStringStream* out, int indent ) const {
		//用tab做缩进
		for ( int i = 0; i < indent; ++i ){
			*out << '\t';
		}
		//标签开始和元素名称
		*out << '<' << mName;
		//写入属性
		//5以个为界改变发行方式。
		int an = mAttributeNumber;
		if ( an >= 5 ){
			*out << "\r\n";
			//有很多换行符
			for ( int i = 0; i < an; ++i ){
				//用tab做缩进
				for ( int j = 0; j < indent; ++j ){
					*out << '\t';
				}
				*out << mAttributes[ i ].name();
				const char* value = mAttributes[ i ].value();
				if ( value ){
					*out << "=\"" << mAttributes[ i ].value() << '"';
				}
				if ( i != an - 1 ){
					*out << "\r\n"; //最后不要换行
				}
			}
		}else{
			for ( int i = 0; i < an; ++i ){
				*out << ' '; //空格
				*out << mAttributes[ i ].name();
				const char* value = mAttributes[ i ].value();
				if ( value ){
					*out << "=\"" << mAttributes[ i ].value() << '"';
				}
			}
		}
		if ( mChildNumber > 0 ){ //
			*out << ">\r\n"; //开始标签结束
			//
			for ( int i = 0; i < mChildNumber; ++i ){
				//如果设置了名称。
				if ( mChildren[ i ].name() ){
					mChildren[ i ].convertToString( out, indent + 1 );
				}
			}
			//用tab做缩进
			for ( int i = 0; i < indent; ++i ){
				*out << '\t';
			}
			//结束标签
			*out << "</" << mName << ">\r\n";
		}else{ //
			*out << "/>\r\n";
		}
	}
	void convertToBinary( 
	OStringStream* out, 
	OStringStream* stringBuffer,
	map< RefString, int >* stringMap ) const {
		typedef map< RefString, int > Map;
		RefString name( mName );
		Map::iterator it = stringMap->find( name );
		int pos = 0;
		if ( it == stringMap->end() ){
			pos = stringBuffer->size();
			*stringBuffer << name;
			stringBuffer->write( "\0", 1 );
			stringMap->insert( make_pair( name, pos ) );
		}else{
			pos = it->second;
		}
		out->write( pos ); //nameOffset
		out->write( mAttributeNumber ); //attributeNumber
		for ( int i = 0; i < mAttributeNumber; ++i ){
			const Attribute::Impl& a = mAttributes[ i ];
			RefString aName( a.name() );
			RefString aValue;
			if ( a.value() ){
				aValue = a.value();
			}
			//name
			it = stringMap->find( aName );
			pos = 0;
			if ( it == stringMap->end() ){
				pos = stringBuffer->size();
				*stringBuffer << aName;
				stringBuffer->write( "\0", 1 );
				stringMap->insert( make_pair( aName, pos ) );
			}else{
				pos = it->second;
			}
			out->write( pos ); //Attribute::nameOffset
			//value
			it = stringMap->find( aValue );
			pos = 0;
			if ( it == stringMap->end() ){
				pos = stringBuffer->size();
				*stringBuffer << aValue;
				stringBuffer->write( "\0", 1 );
				stringMap->insert( make_pair( aValue, pos ) );
			}else{
				pos = it->second;
			}
			out->write( pos ); //Attribute::valueOffset
		}
		//给孩子
		out->write( mChildNumber ); //childNumber
		for ( int i = 0;i < mChildNumber; ++ i ){
			if ( mChildren[ i ].name() ){ //
				mChildren[ i ].convertToBinary( out, stringBuffer, stringMap );
			}
		}
	}
	int attributeNumber() const {
		return mAttributeNumber;
	}
	void setAttributeNumber( int n ){
		ASSERT( mAttributeNumber == 0 );
		mAttributeNumber = n;
		mAttributes = mDocument->allocateAttribute( n );
		for ( int i = 0; i < mAttributeNumber; ++i ){
			mAttributes[ i ].setDocument( mDocument );
		}
	}
	Attribute::Impl* attribute( int i ){
		ASSERT( i >= 0 && i < mAttributeNumber );
		return &mAttributes[ i ];
	}
	const Attribute::Impl* attribute( int i ) const {
		return &mAttributes[ i ];
	}
	int childNumber() const {
		return mChildNumber;
	}
	void setChildNumber( int n ){
		ASSERT( mChildNumber == 0 );
		mChildNumber = n;
		mChildren = mDocument->allocateElement( n );
		for ( int i = 0; i < mChildNumber; ++i ){
			mChildren[ i ].setDocument( mDocument );
		}
	}
	Element::Impl* child( int i ){
		return &mChildren[ i ];
	}
	const Element::Impl* child( int i ) const {
		return &mChildren[ i ];
	}
	const char* name() const {
		return mName;
	}
	void setName( const char* name ){
		mName = mDocument->allocateString( name );
	}
	void setNameReference( const char* name ){
		mName = name;
	}
	void setDocument( Document::Impl* d ){
		mDocument = d;
	}
	Document::Impl* document(){
		return mDocument;
	}
private:
	Element::Impl* mChildren;
	int mChildNumber;
	Attribute::Impl* mAttributes;
	int mAttributeNumber;
	const char* mName;
	Document::Impl* mDocument;
};

} //namespace PseudoXml
} //namespace GameLib
#endif
