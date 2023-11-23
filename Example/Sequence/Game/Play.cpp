#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "Game/State.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{
namespace Game{

Play::Play(){
	//BGM开始
	SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
}

Play::~Play(){ 
	//停止背景音乐
	SoundManager::instance()->stopBgm();
}

//游戏本体
void Play::update( Parent* parent ){
	Input::Keyboard kb = Input::Manager::instance().keyboard();

	State* state = parent->state();

	bool cleared = state->hasCleared();
	bool die1P = !state->isAlive( 0 );
	bool die2P = !state->isAlive( 1 );
	//使用debug命令行测试。
	if ( kb.isTriggered( '1' ) ){ //2P杀人
		die2P = true;
	}else if ( kb.isTriggered( '2' ) ){ //1P杀人
		die1P = true;
	}else if ( kb.isTriggered( 'c' ) ){
		cleared = true;
	}
	//如果按空格键，则暂停
	//清除后报告
	if ( parent->mode() == Parent::MODE_1P ){
		if ( cleared && !die1P ){
			parent->moveTo( Parent::NEXT_CLEAR );
		}else if ( die1P ){
			parent->moveTo( Parent::NEXT_FAILURE );
		}
	}else{ //双人
		if ( die1P || die2P ){ //如果任何一方死亡，则判断胜负
			parent->moveTo( Parent::NEXT_JUDGE );
			if ( die1P && die2P ){
				parent->setWinner( Parent::PLAYER_NONE ); //都死了
			}else if ( die1P ){
				parent->setWinner( Parent::PLAYER_2 );
			}else{
				parent->setWinner( Parent::PLAYER_1 );
			}
		}
	}
	//使用B按钮暂停
	if ( Pad::isTriggered( Pad::B ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//更新
	state->update();
	//绘制
	state->draw();
}

} //namespace Game
} //namespace Sequence
