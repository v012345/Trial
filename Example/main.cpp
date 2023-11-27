#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"

#include <sstream>
using namespace std;

double gZMove = 5000.0;
bool gFirstFrame = true;
GameLib::Texture* gTexture;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		if ( gFirstFrame ){
			gFirstFrame = false;
			createTexture( &gTexture,CMAKE_CURRENT_SOURCE_DIR "grid.tga" );
		}

		double p[ 4 ][ 3 ] = { 
			{ -500.0, 300.0, -1000.0 },
			{ -500.0, 300.0, 1000.0 },
			{ 500.0, 300.0, -1000.0 },
			{ 500.0, 300.0, 1000.0 },
		};
		double uv[ 4 ][ 2 ] = { 
			{ 0.0, 0.0 },
			{ 1.0, 0.0 },
			{ 0.0, 1.0 },
			{ 1.0, 1.0 },
		};

		//将x和y乘以640，除以z，然后加上（320,240）。将z从[1,10000]转换为[0,1]。
		for ( int i = 0; i < 4; ++i ){
			//反映移动量
			p[ i ][ 2 ] += gZMove;
			//放大，除以z
			p[ i ][ 0 ] *= 640.0 / p[ i ][ 2 ];
			p[ i ][ 1 ] *= 640.0 / p[ i ][ 2 ];
			//中心偏移
			p[ i ][ 0 ] += 320.f;
			p[ i ][ 1 ] += 240.f;
			//z范围转换
			p[ i ][ 2 ] = ( 1.0/9999.0 ) * p[ i ][ 2 ] - ( 1.0/9999.0 );
		}
		//画一个矩形。
		setTexture( gTexture );
		drawTriangle3D( p[ 0 ], p[ 1 ], p[ 2 ], uv[ 0 ], uv[ 1 ], uv[ 2 ], 0xffff0000, 0xffff0000, 0xffff0000 );
		drawTriangle3D( p[ 3 ], p[ 1 ], p[ 2 ], uv[ 3 ], uv[ 1 ], uv[ 2 ], 0xff00ff00, 0xff00ff00, 0xff00ff00 );

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
