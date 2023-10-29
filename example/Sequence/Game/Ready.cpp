#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Ready::Ready() : 
mImage( 0 ), mCount( 0 ), mStarted( false ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR"data/image/dummy.dds" );
}

Ready::~Ready(){
	SAFE_DELETE( mImage );
}

void Ready::update( Parent* parent ){
	if ( !mStarted ){
		parent->startLoading();
		mStarted = true;
	}
	if ( mCount >= 120 ){ //2过了一秒就到Play
		parent->moveTo( Parent::NEXT_PLAY );
	}else if ( mCount >= 60 ){ //显示Go
		Framework::instance().drawDebugString( 0, 1, "GO!" );
	}else{ //显示Ready...
		Framework::instance().drawDebugString( 0, 1, "Ready..." );
	}
	//绘制
	parent->drawState();
	mImage->draw();
	//输出文字
	Framework::instance().drawDebugString( 0, 0, "[READY] : " );

	++mCount;
}

} //namespace Game
} //namespace Sequence