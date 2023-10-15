#ifndef INCLUDED_GAMELIB_FILEIO_OUTFILE_H
#define INCLUDED_GAMELIB_FILEIO_OUTFILE_H

namespace GameLib{
namespace FileIO{

class OutFile{
public:
	static OutFile create( const char* filename, const char* data, int size );
	///保存结束了吗？
	bool isFinished() const;
	///发生错误了吗？
	bool isError() const;

	//用户不会意识到的函数
	OutFile();
	OutFile( const OutFile& );
	~OutFile();
	operator void*() const;
	OutFile& operator=( const OutFile& );
	void release();
	bool operator==( const OutFile& ) const;
	bool operator!=( const OutFile& ) const;

	class Impl;
private:
	Impl* mImpl;
};

} //namespace FileIO
} //namespace GameLib

#endif
