#ifndef INCLUDED_GAMELIB_INPUT_MANAGER_H
#define INCLUDED_GAMELIB_INPUT_MANAGER_H

namespace GameLib{
namespace Math{
	class Vector2;
}
namespace Input{
class Keyboard;
class Mouse;
class Joystick;

class Manager{
public:
	Manager();
	static Manager instance();

	Mouse mouse() const;
	Keyboard keyboard() const;
	Joystick joystick( int i = 0 ) const;
	int joystickNumber() const;

	//你不需要知道
	static void create( void* windowHandle );
	static void destroy();
	void update( float pointerScale, const Math::Vector2& pointerOffset );
};

} //namespace Input
} //namespace GameLib

#endif
