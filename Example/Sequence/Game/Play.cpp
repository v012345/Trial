#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "State.h"

using namespace GameLib;

namespace Sequence{
namespace Game{

Play::Play(){ //什么都不做
}

Play::~Play(){ //什么都不做
}

//游戏本体
void Play::update( Parent* parent ){
	State* state = parent->state();

	bool cleared = false;
	//主循环
	//清除检测
	if ( state->hasCleared() ){
		cleared = true;
	}
	//获取输入
	//按下空格键进入菜单
	Framework f = Framework::instance();
	if ( f.isKeyTriggered( ' ' ) ){
		parent->moveTo( Parent::SEQ_MENU );
	}else{ //通常更新
		int dx = 0;
		int dy = 0;
		if ( f.isKeyOn( 'a' ) ){
			dx -= 1;
		}else if ( f.isKeyOn( 's' ) ){
			dx += 1;
		}else if ( f.isKeyOn( 'w' ) ){
			dy -= 1;
		}else if ( f.isKeyOn( 'z' ) ){
			dy += 1;
		}
		//更新
		state->update( dx, dy );

		//清除后报告
		if ( cleared ){
			parent->moveTo( Parent::SEQ_CLEAR );
		}
	}
	//绘制
	state->draw();
}

} //namespace Game
} //namespace Sequence
