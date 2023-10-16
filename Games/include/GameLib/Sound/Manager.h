#ifndef INCLUDED_GAMELIB_SOUND_MANAGER_H
#define INCLUDED_GAMELIB_SOUND_MANAGER_H

namespace GameLib{
namespace Sound{

class Manager{
public:
	Manager();
	static Manager instance();

	//用户未调用
	static void create( void* windowHandle );
	static void destroy();
};

} //namespace Sound
} //namespace GameLib

#endif
