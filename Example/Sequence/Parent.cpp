#include "GameLib/GameLib.h"

#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/StageSelect.h"
#include "Sequence/Title.h"
#include "StringRenderer.h"

namespace Sequence{

Parent::Parent() : 
mTitle( 0 ),
mStageSelect( 0 ),
mGame( 0 ),
mNext( SEQ_NONE ),
mStageID( 0 ){
	//字符串类初始化
	StringRenderer::create( CMAKE_CURRENT_SOURCE_DIR "data/image/font.dds" );
	//
	mTitle = new Title();
}

Parent::~Parent(){
	//如果剩余则销毁
	SAFE_DELETE( mTitle );
	SAFE_DELETE( mStageSelect );
	SAFE_DELETE( mGame );
	//字符串类结束
	StringRenderer::destroy();
}

void Parent::update(){
	if ( mTitle ){
		mTitle->update( this );
	}else if ( mStageSelect ){
		mStageSelect->update( this );
	}else if ( mGame ){
		mGame->update( this );
	}else{
		HALT( "bakana!" ); //不可能的
	}
	//迁移判断
	switch ( mNext ){
		case SEQ_STAGE_SELECT:
			SAFE_DELETE( mTitle );
			SAFE_DELETE( mGame );
			mStageSelect = new StageSelect();
			break;
		case SEQ_TITLE:
			SAFE_DELETE( mGame );
			mTitle = new Title(); 
			break;
		case SEQ_GAME:
			SAFE_DELETE( mStageSelect );
			ASSERT( mStageID != 0 ); //应该正确放入
			mGame = new Game::Parent( mStageID );
			break;
	}
	mNext = SEQ_NONE;
}


void Parent::moveTo( SeqID next ){
	mNext = next;
}

void Parent::setStageID( int stageID ){
	mStageID = stageID;
}

} //namespace Sequence