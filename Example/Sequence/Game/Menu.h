#ifndef INCLUDED_SEQUENCE_GAME_MENU_H
#define INCLUDED_SEQUENCE_GAME_MENU_H

class Image;

namespace Sequence{
namespace Game{
class Parent;

class Menu{
public:
	Menu();
	~Menu();
	void update( Parent* );
private:
	Image* mImage;
	int mMenuCursor; //光标位置
};

} //namespace Game
} //namespace Sequence

#endif