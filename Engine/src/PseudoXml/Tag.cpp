#include "GameLib/GameLib.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/AttributeImpl.h"
#include "PseudoXml/DocumentImpl.h"
#include "GameLib/Base/RefString.h"
using namespace std;

namespace GameLib{
namespace PseudoXml{

//状态迁移表。

Tag::Tag( const char** p, const char* e, Document::Impl* document ) : 
mType( TYPE_BEGIN ),
mDocument( document ){
	//临时存储元素名称，属性名称和值
	const char* eName = 0;
	int eNameSize = 0;
	const char* aName = 0;
	int aNameSize = 0;
	const char* aValue = 0;
	int aValueSize = 0;

	int m = 0; //模式。使用模式会更好
	bool end = false; //标记退出循环
	while ( *p < e ){
		bool wide = false; //是日语吗？
		char c = **p;
		switch ( m ){
			case 0: //初始状态
				if ( c == '/' ){
					mType = TYPE_END;
				}else if ( c == '!' ){ //注释
					mType = TYPE_COMMENT;
					m = 10;
				}else if ( c == '?'){
					mType = TYPE_COMMENT;
					m = 13;
				}else if ( isNormalChar( c, &wide ) ){
					eName = *p;
					eNameSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++eNameSize;
					}
					m = 1;
				}else{
					; //什么都不做
				}
				break;
			case 1: //元素名
				if ( c == '>' ){
					end = true;
				}else if ( isNormalChar( c, &wide ) ){
					++eNameSize;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++eNameSize;
					}
				}else{
					m = 2; //去掉元素名称
				}
				break;
			case 2: //元素名称后的空格
				if ( c == '>' ){
					end = true;
				}else if ( c == '/' ){
					mType = TYPE_BEGIN_END; //空元素标签
					m = 6;
				}else if ( isNormalChar( c, &wide ) ){
					aName = *p;
					aNameSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aNameSize;
					}
					m = 3;
				}else{
					; //什么都不做
				}
				break;
			case 3: //属性名称
				if ( c == '=' ){
					m = 4;
				}else if ( isNormalChar( c, &wide ) ){
					++aNameSize;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aNameSize;
					}
				}else{
					m = 7; //空白
				}
				break;
			case 4: //=上
				if ( c == '"' ){
					aValue = *p + 1;
					m = 5;
				}else if ( c == '\''){
					aValue = *p + 1;
					m = 8;
				}else if ( isNormalChar( c, &wide ) ){
					aValue = *p;
					aValueSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
					m = 9;
				}else{
					; //什么都不做
				}
				break;
			case 5: //属性值
				if ( c == '"' ){
					m = 2; //元素名称后的空格
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}else{
					++aValueSize;
					isNormalChar( c, &wide ); //只想知道它是否够宽
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
				}
				break;
			case 6: //空元素标签/出现后
				switch ( c ){
					case '>': end = true; break;
					default: break; //
				}
				break;
			case 7: //属性名称后的空格
				if ( c == '=' ){
					m = 4;
				}else if ( c == '>' ){
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
					end = true;
				}else if ( isNormalChar( c, &wide ) ){ //没有值的属性
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
					//放入下一个属性
					aName = *p;
					aNameSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aNameSize;
					}
					m = 3;
				}else{
					; //什么都不做
				}
				break;
			case 8: //属性值
				if ( c == '\'' ){
					m = 2; //元素名称后的空格
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}else{
					++aValueSize;
					isNormalChar( c, &wide ); //只想知道它是否够宽
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
				}
				break;
			case 9: //属性值
				if ( c == '/' ){
					mType = TYPE_BEGIN_END; //空元素标签
					m = 6;
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}else if ( c == '>' ){
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
					end = true;
				}else if ( isNormalChar( c, &wide ) ){
					++aValueSize;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
				}else{
					m = 2; //元素名称后的空格
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}
			case 10: //注释
				switch ( c ){
					case '-': m = 11; break;
					default: break; //
				}
				break;
			case 11: //注释
				switch ( c ){
					case '-': m = 12; break;
					default: m = 10; break; //注释继续
				}
				break;
			case 12: //注释
				switch ( c ){
					case '>': end = true; break;
					default: m = 10; break; //注释继续
				}
				break;
			case 13: //？注释
				switch ( c ){
					case '?': m = 14; break;
					default: break; //
				}
				break;
			case 14: //？注释
				switch ( c ){
					case '>': end = true; break;
					default: m = 13; break; //注释继续
				}
				break;
		}
		*p += ( wide ) ? 2 : 1; //前进
		if ( end ){ //结束了
			break;
		}
	}
	mName.set( eName, eNameSize );
}

} //namespace PseudoXml
} //namespace GameLib
