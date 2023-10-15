#ifndef INCLUDED_GAMELIB_FILEIO_MANAGER_H
#define INCLUDED_GAMELIB_FILEIO_MANAGER_H

#include <string>

namespace GameLib{
namespace FileIO{

class Manager{
public:
	//如何使用档案
	enum AccessMode{
		MODE_ARCHIVE_FIRST, //存档优先级。
		MODE_DIRECT_FIRST, //优先直接访问
		MODE_ARCHIVE_ONLY, //仅存档。发布时最好保留这个。
	};
	///获取实例。也可以只写Manager().foo()
	static Manager instance();
	///不接收handler而进行写入。无法区分成功与失败
	void write( const char* filename, const char* data, int size );
	///把装载的东西列到列表里吐出来。用于调试。防止遗漏。
	std::string createListString() const;
	///容量界限的指定。超过warning时警告，超过error时出错。标准值均为2GB
	void setLimit( int warning, int error );
	///是否在发生错误时停止。默认是true。如果觉得不好，就改成false
	void enableHaltOnError( bool );

	//以下用户不会意识到
	static void create( 
		const char** archiveNames = 0, 
		int archiveNumber = 0, 
		AccessMode = MODE_DIRECT_FIRST );
	static void destroy();
};

} //namespace Sound
} //namespace GameLib

#endif
