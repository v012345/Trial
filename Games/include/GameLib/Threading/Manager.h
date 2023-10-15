#ifndef INCLUDED_GAMELIB_THREADING_MANAGER_H
#define INCLUDED_GAMELIB_THREADING_MANAGER_H

namespace GameLib{
namespace Threading{

class Manager{
public:
	Manager();
	static Manager instance();

	//获取cpu核数（因为它是逻辑核心数，所以可能与实际数不同）
	int getCoreNumber() const;

	//用户未调用
	static void create( int additionalThreadNumber );
	static void destroy();
};

} //namespace Threading
} //namespace GameLib

#endif
