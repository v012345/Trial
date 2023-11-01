#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "State.h"
#include "StringRenderer.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

void Clear::update( Parent* parent ){
	if ( mCount == 60 ){ //60按框架移动
		parent->moveTo( Parent::SEQ_STAGE_SELECT );
	}

	//绘制
	//首先绘制游戏画面
	parent->state()->draw();
	//在顶部绘制庆祝消息
	mImage->draw();
	//画清楚和字母
	StringRenderer::instance()->draw( 0, 0, "CLEAR!" );

	++mCount;
}

} //namespace Game
} //namespace Sequence
