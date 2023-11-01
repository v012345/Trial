#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Loading.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "StringRenderer.h"

namespace Sequence{
namespace Game{

//由于加载是在update开始的，因此加载不在此处开始，
//“加载中”的图像正在加载
Loading::Loading() : 
mImage( 0 ),
mCount( 0 ),
mStarted( false ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds" );
}

Loading::~Loading(){
	SAFE_DELETE( mImage );
}

void Loading::update( Parent* parent ){
	if ( !mStarted ){
		//
		parent->startLoading();
		mStarted = true;
	}else if ( mCount == 60 ){ //1等待秒
		parent->moveTo( Parent::SEQ_PLAY ); //迁移到Play
	}

	//绘制
	mImage->draw();
	//输出文字
	StringRenderer::instance()->draw( 0, 0, "NOW LOADING..." );

	++mCount;
}

} //namespace Game
} //namespace Sequence