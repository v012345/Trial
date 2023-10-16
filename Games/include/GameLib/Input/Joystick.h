#ifndef INCLUDED_GAMELIB_INPUT_JOYSTICK_H
#define INCLUDED_GAMELIB_INPUT_JOYSTICK_H

namespace GameLib{
namespace Input{

class Joystick{
public:
	enum Button{
		BUTTON_UP = 128,
		BUTTON_DOWN,
		BUTTON_LEFT,
		BUTTON_RIGHT,
	};

	int analogNumber() const;
	int analog( int ) const; //获取手柄输入
	//可以从上面的列表中获得顶部，底部，左侧和右侧。其他按钮是从0开始的数字。
	bool isOn( int ) const;
	bool isTriggered( int ) const;
	int buttonNumber() const;
	//有效吗
	bool isEnabled() const;

	Joystick();
	~Joystick();
	operator void*() const;
	class Impl;
private:
	friend class Manager;
	Joystick( Impl* );
	Impl* mImpl;
};

} //namespace Input
} //namespace GameLib

#endif
