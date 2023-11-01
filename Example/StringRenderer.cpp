#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "StringRenderer.h"
#include "Image.h"

//静态变量需要cpp中的实体
StringRenderer* StringRenderer::mInstance = 0;

StringRenderer::StringRenderer( const char* fontFileName ) : mImage( 0 ){
	mImage = new Image( fontFileName );
}

StringRenderer::~StringRenderer(){
	SAFE_DELETE( mImage );
}

void StringRenderer::create( const char* fontFileName ){
	//连续调用2次是不对的。会崩溃
	STRONG_ASSERT( !mInstance && "StringRenderer::create() called twice!" );
	mInstance = new StringRenderer( fontFileName );
}

void StringRenderer::destroy(){
	SAFE_DELETE( mInstance );
}

StringRenderer* StringRenderer::instance(){
	return mInstance;
}

void StringRenderer::draw( int x, int y, const char* s, unsigned c ) const {
	//两个常数。
	const int CHAR_WIDTH = 8;
	const int CHAR_HEIGHT = 16;
	//初始化写入目的地
	int dstX = x * CHAR_WIDTH;
	int dstY = y * CHAR_HEIGHT;
	//获取框架
	Framework f = Framework::instance();
	//范围检查
	if ( dstY < 0 || dstY >= ( int )f.height() ){
		return;
	}
	//图像宽度
	for ( int i = 0; s[ i ] != '\0'; ++i ){
		//X范围判断
		if ( dstX >= 0 && ( dstX + CHAR_WIDTH ) < f.width() ){
			//字符范围检查
			int t = s[ i ];
			if ( t < 32 || t >= 128 ){
				t = 127; //
			}
			t -= 32; //减去32
			int srcX = ( t % 16 ) * CHAR_WIDTH; //X是除以宽度的余数
			int srcY = ( t / 16 ) * CHAR_HEIGHT; //Y是除以宽度后的商
			mImage->drawWithFixedColor( dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT, c );
		}
		dstX += CHAR_WIDTH;
	}
}



