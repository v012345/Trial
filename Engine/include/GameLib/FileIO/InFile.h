#ifndef INCLUDED_GAMELIB_FILEIO_INFILE_H
#define INCLUDED_GAMELIB_FILEIO_INFILE_H

namespace GameLib{
namespace FileIO{

///文件读取处理器
class InFile{
public:
	///开始加载文件
	static InFile create( const char* filename );
	///获得装入容量
	int size() const;
	///获得下载内容
	const char* data() const; 
	///载入结束了吗？
	bool isFinished() const;
	///发生错误了吗？
	bool isError() const;
	//获取值的函数
	int getInt( int position ) const;
	unsigned getUnsigned( int position ) const;
	short getShort( int position ) const;
	unsigned short getUnsignedShort( int position ) const;

	//用户不会意识到的函数
	InFile();
	InFile( const InFile& );
	~InFile();
	operator void*() const;
	InFile& operator=( const InFile& );
	void release();
	bool operator==( const InFile& ) const;
	bool operator!=( const InFile& ) const;

	class Impl;
private:
	Impl* mImpl;
};

} //namespace FileIO
} //namespace GameLib

#endif
