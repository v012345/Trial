#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/GameOver.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Failure::Failure() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR"data/image/dummy.dds" );
}

Failure::~Failure(){
	SAFE_DELETE( mImage );
}

Base* Failure::update( Parent* parent ){
	Base* next = this;
	if ( mCount == 60 ){ //1等待秒
		if ( parent->lifeNumber() == 0 ){
			next = new GameOver;
		}else{
			next = new Ready;
		}
	}
	//绘制
	//首先绘制游戏画面
	parent->drawState();
	//在顶部绘制庆祝消息
	mImage->draw();
	//画清楚和字母
	Framework::instance().drawDebugString( 0, 0, "Ah!" );

	++mCount;

	return next;
}

} //namespace Game
} //namespace Sequence
