#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Judge::Judge() : mImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR"data/image/dummy.dds" );
}

Judge::~Judge(){
	SAFE_DELETE( mImage );
}

void Judge::update( Parent* parent ){
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //
 			mCursorPosition = 1;
		}
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //1越过0就循环成0
			mCursorPosition = 0;
		}
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosition == 0 ){
			//
			parent->moveTo( Parent::NEXT_READY );
		}else if ( mCursorPosition == 1 ){
			//到主题
			parent->moveTo( Parent::NEXT_TITLE );
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
	f.drawDebugString( 1, 3, "mada" );//
	f.drawDebugString( 1, 4, "yamete" );//
	//写光标

	f.drawDebugString( 0, mCursorPosition + 3, ">" );
}



} //namespace Game
} //namespace Sequence