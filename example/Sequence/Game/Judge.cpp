#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/Title.h"

namespace Sequence{
namespace Game{

Judge::Judge() : mImage( 0 ), mCursorPosistion( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR"data/image/dummy.dds" );
}

Judge::~Judge(){
	SAFE_DELETE( mImage );
}

Base* Judge::update( Parent* parent ){
	Base* next = this;
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosistion;
		if ( mCursorPosistion < 0 ){ //
 			mCursorPosistion = 1;
		}
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosistion;
		if ( mCursorPosistion > 1 ){ //1越过0就循环成0
			mCursorPosistion = 0;
		}
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosistion == 0 ){
			//
			next = new Ready;
		}else if ( mCursorPosistion == 1 ){
			//到主题
			next = new Title;
		}
	}
	//绘制
	//首先是游戏画面
	parent->drawState();
	//
	mImage->draw();

	//显示哪个赢了
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[show result]" );

	Parent::PlayerID winner = parent->winner();
	if ( winner == Parent::PLAYER_1 ){
		f.drawDebugString( 0, 1, "1P win!" );
	}else if ( winner == Parent::PLAYER_2 ){
		f.drawDebugString( 0, 1, "2P win!" );
	}else{
		f.drawDebugString( 0, 1, "nobody win" );
	}
	//菜单
	f.drawDebugString( 1, 3, "game continue" );
	f.drawDebugString( 1, 4, "return title" );
	//写光标
	f.drawDebugString( 0, mCursorPosistion + 3, ">" );

	return next;
}



} //namespace Game
} //namespace Sequence