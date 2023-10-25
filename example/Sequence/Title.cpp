#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "Pad.h"

namespace Sequence{

Title::Title() : mImage( 0 ), mCursorPosistion( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR"data/image/dummy.dds" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
}

Base* Title::update( Parent* parent ){
	Base* next = this;
	//获取输入
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
		next = new Game::Parent( parent->mode() );
		if ( mCursorPosistion == 0 ){
			parent->setMode( Parent::MODE_1P );
		}else if ( mCursorPosistion == 1 ){
			parent->setMode( Parent::MODE_2P );
		}else{
			HALT( "arienai" );
		}
	}
	//绘制
	mImage->draw();
	//输出文字
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[title] : boom man" );
	f.drawDebugString( 1, 2, "one player" );
	f.drawDebugString( 1, 3, "two players" );
	//写光标
	f.drawDebugString( 0, mCursorPosistion + 2, ">" );

	return next;
}

} //namespace Sequence