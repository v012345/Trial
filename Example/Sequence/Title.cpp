#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{

Title::Title() : mImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
}

void Title::update( Parent* parent ){
	//获取输入
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //
 			mCursorPosition = 1;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //1越过0就循环成0
			mCursorPosition = 0;
		}
		//光标声音
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		parent->moveTo( Parent::NEXT_GAME );
		if ( mCursorPosition == 0 ){
			parent->setMode( Parent::MODE_1P );
		}else if ( mCursorPosition == 1 ){
			parent->setMode( Parent::MODE_2P );
		}else{
			HALT( "arienai" );
		}
		//决定音
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//绘制
	mImage->draw();
	//输出文字
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[title] : boom man" );//
	f.drawDebugString( 1, 2, "one player" );//
	f.drawDebugString( 1, 3, "two players" );//
	//写光标
	f.drawDebugString( 0, mCursorPosition + 2, ">" );
}

} //namespace Sequence