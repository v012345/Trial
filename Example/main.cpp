#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"

#include <sstream>
using namespace std;

double gZMove = 2000.0;
bool gFirstFrame = true;
GameLib::Texture* gTexture;

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );
		if ( gFirstFrame ){
			gFirstFrame = false;
			createTexture( &gTexture,CMAKE_CURRENT_SOURCE_DIR "grid.tga" );
		}

		double p[ 4 ][ 4 ] = { //4维数
			{ -500.0, -300.0, -1000.0, 1.0 }, //Y朝上正方向，所以向下移动是负数
			{ -500.0, -300.0, 1000.0, 1.0 },
			{ 500.0, -300.0, -1000.0, 1.0 },
			{ 500.0, -300.0, 1000.0, 1.0 },
		};
		double uv[ 4 ][ 2 ] = { 
			{ 0.0, 0.0 },
			{ 1.0, 0.0 },
			{ 0.0, 1.0 },
			{ 1.0, 1.0 },
		};

		//准备移交给硬件
		for ( int i = 0; i < 4; ++i ){
			//将y乘上640/480以调整宽高比
			p[ i ][ 1 ] *= 640.0 / 480.0;
			//反映移动量
			p[ i ][ 2 ] += gZMove;
			//将范围转换之前的z存储在w中
			p[ i ][ 3 ] = p[ i ][ 2 ];
			//z范围转换
			p[ i ][ 2 ] = ( 1.0/9999.0 ) * p[ i ][ 2 ] - ( 1.0/9999.0 );
			//在范围转换之前对z处理
			p[ i ][ 2 ] *= p[ i ][ 3 ];
		}
		//画一个矩形。
		setTexture( gTexture );
		drawTriangle3DH( p[ 0 ], p[ 1 ], p[ 2 ], uv[ 0 ], uv[ 1 ], uv[ 2 ], 0xffff0000, 0xffff0000, 0xffff0000 );
		drawTriangle3DH( p[ 3 ], p[ 1 ], p[ 2 ], uv[ 3 ], uv[ 1 ], uv[ 2 ], 0xff00ff00, 0xff00ff00, 0xff00ff00 );

		if ( Input::Manager::instance().keyboard().isOn( 's' ) ){
			gZMove -= 25.f;
		}else if ( Input::Manager::instance().keyboard().isOn( 'w' ) ){
			gZMove += 25.f; 
		}
		//显示
		ostringstream oss;
		oss << "[w-s] Z MOVEMENT: " << gZMove;
		drawDebugString( 10, 0, oss.str().c_str() );
		oss.str( "" );

		if ( isEndRequested() ){
			destroyTexture( &gTexture );
		}
	}
}
