#ifndef INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H
#define INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H

#include <string>

namespace GameLib{
template< class T > class Array;
namespace PseudoXml{
using namespace std;

class Element;
class ConstElement;

///具有文件访问功能的伪xml类
class Document{
public:
	///创建用于后续写入
	static Document create();
	///通过文件构造（要初始化FileIO）
	static Document create( const char* filename );
	///从已装入字符串开始构造
	static Document create( const char* data, int size );
	///获取根Element
	Element root();
	///获取根Element
	ConstElement root() const; 
	///是否已载入
	bool isReady();
	///读取时出错吗？
	bool isError() const;
	///生成伪xml文本
	void convertToString( string* out ) const;
	///生成伪xml二进制
	void convertToBinary( Array< char >* out ) const;
	///生成伪xml文件（需要初始化FileIO）
	void write( const char* filename, bool isBinary = false ) const;
	///获得子元素数量
	int childNumber() const;
	///获得子元素数量
	Element child( int i );
	///获得子元素数量
	ConstElement child( int i ) const;

	//不需要知道
	Document();
	Document( const Document& );
	~Document();
	operator void*() const;
	Document& operator=( const Document& );
	void release();
	bool operator==( const Document& ) const;
	bool operator!=( const Document& ) const;

	class Impl;
private:
	Impl* mImpl;
};

//const版本
class ConstDocument{
public:
	///获取根Element
	ConstElement root() const; 
	///读取时出错吗？
	bool isError() const;
	///生成伪xml文本
	void convertToString( string* out ) const;
	///生成伪xml二进制
	void convertToBinary( Array< char >* out ) const;
	///生成伪xml文件（需要初始化FileIO）
	void write( const char* filename, bool isBinary = false ) const;
	///获得子元素数量
	int childNumber() const;
	///获得子元素数量
	ConstElement child( int i ) const;

	//你不需要知道
	ConstDocument();
	ConstDocument( const Document& );
	~ConstDocument();
	ConstDocument& operator=( const Document& );
	void release();
	operator void*() const;
	bool operator==( const Document& ) const;
	bool operator!=( const Document& ) const;
private:
	Document mBase;
};

} //namespace PseudoXml
} //namespace GameLib

#endif
