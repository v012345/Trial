#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Games/Framework.h"
#include "Image.h"
#include "State.h"
#include "Sequence/Game/Menu.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Menu::Menu() : mImage( 0 ){
	mImage = new Image( "data/image/menu.dds" );
}

Menu::~Menu(){
	SAFE_DELETE( mImage );
}

void Menu::update( Parent* parent ){
	//1：重做
	//2:面对面硒
	//3：转到标题画面
	//4：返回
	//如果下标移位，则很麻烦，因此插入0
	char numberChars[] = { '0', '1', '2', '3', '4' };
	int inputNumber = 0;
	for ( int i = 0; i < 5; ++i ){
		if ( Framework::instance().isKeyTriggered( numberChars[ i ] ) ){
			inputNumber = i;
		}
	}
	switch ( inputNumber ){
		case 1: //重来
			parent->state()->reset();
			parent->moveTo( Parent::SEQ_PLAY );
			break;
		case 2: //
			parent->moveTo( Parent::SEQ_STAGE_SELECT );
			break;
		case 3: //到主题
			parent->moveTo( Parent::SEQ_TITLE );
			break;
		case 4: //照原样返回
			parent->moveTo( Parent::SEQ_PLAY );
			break;
		default: //忽略其他
			break;
	}
	//绘制
	//首先是游戏画面
	parent->state()->draw();
	//
	mImage->draw();
}



} //namespace Game
} //namespace Sequence