#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/StageSelect.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "StringRenderer.h"
#include "sstream"
using namespace std;

namespace Sequence{

StageSelect::StageSelect() : mImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds" );
}

StageSelect::~StageSelect(){
	SAFE_DELETE( mImage );
}

void StageSelect::update( Parent* parent ){
	//获取输入
	Framework f = Framework::instance();
	if ( f.isKeyTriggered( 'w' ) ){
		--mCursorPosition;
		if ( mCursorPosition < 1-1 ){ //
 			mCursorPosition = 9-1;
		}
	}else if ( f.isKeyTriggered( 'z' ) ){
		++mCursorPosition;
		if ( mCursorPosition > 9-1 ){ //超过9面则循环到1面
			mCursorPosition = 1-1;
		}
	}else if ( f.isKeyTriggered( ' ' ) ){
		parent->setStageID( mCursorPosition + 1 ); //1加上了舞台号码
		parent->moveTo( Parent::SEQ_GAME );
	}
	//绘制
	mImage->draw();
	//
	StringRenderer* sr = StringRenderer::instance();
	sr->draw( 0, 0, "STAGE SELECTION" );
	ostringstream oss;
	for ( int i = 1; i <= 9; ++i ){
		oss << i;
		sr->draw( 1, i, oss.str().c_str() );
		oss.str( "" );
	}
	//写光标
	sr->draw( 0, mCursorPosition + 1, ">" );
}

} //namespace Sequence