#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

void Clear::update( Parent* parent ){
	if ( mCount == 60 ){ //1等待秒
		if ( parent->hasFinalStageCleared() ){ //
			parent->moveTo( Parent::NEXT_ENDING ); 
		}else{
			parent->moveTo( Parent::NEXT_READY );
		}
	}
	//绘制
	//首先绘制游戏画面
	parent->drawState();
	//在顶部绘制庆祝消息
	mImage->draw();
	//画清楚和字母
	Framework::instance().drawDebugString( 0, 0, "clear!" );

	++mCount;
}

} //namespace Game
} //namespace Sequence
