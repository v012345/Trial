#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Sequence/GameOver.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "Image.h"

namespace Sequence{

GameOver::GameOver() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR"data/image/dummy.dds" );
}

GameOver::~GameOver(){
	SAFE_DELETE( mImage );
}

Base* GameOver::update( Parent* ){
	Base* next = this;
	if ( mCount == 60 ){ //1等待秒
		next = new Title;
	}
	//绘制
	mImage->draw();
	//输出文字
	Framework::instance().drawDebugString( 0, 0, "game over" );

	++mCount;

	return next;
}

} //namespace Sequence