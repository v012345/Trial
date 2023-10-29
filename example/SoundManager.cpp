#include "SoundManager.h"
#include "GameLib/GameLib.h"
#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"
#include <sstream>
using namespace GameLib;

SoundManager* SoundManager::mInstance = 0;

SoundManager* SoundManager::instance(){
	return mInstance;
}

void SoundManager::create(){
	ASSERT( !mInstance );
	mInstance = new SoundManager();
}

void SoundManager::destroy(){
	ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

SoundManager::SoundManager() :
mSePlayerPos( 0 ){
	//开始加载文件名列表。
	//为了使其正确执行，文件名列表应为单独的文件，
	//或将其加载到文本文件中。

	//由于这是一个demo，因此所有文件都是相同的...
	const char* bgmFiles[] = {
		"charara.wav", //BGM_TITLE
		"charara.wav", //BGM_GAME
	};
	const char* seFiles[] = {
		"dokaan.wav", //SE_EXPLOSION
		"dokaan.wav", //SE_DEATH
		"dokaan.wav", //SE_CURSOR_MOVE
		"dokaan.wav", //SE_SELECTION
		"dokaan.wav", //SE_SET_BOMB
	};
	std::ostringstream oss; //用于字符串合成的字符串流
	for ( int i = 0; i < BGM_MAX; ++i ){
		oss.str( "" ); //初始化
		oss << CMAKE_CURRENT_SOURCE_DIR"data/sound/bgm/" << bgmFiles[ i ];
		mBgmWaves[ i ] = Sound::Wave::create( oss.str().c_str() );
	}
	for ( int i = 0; i < SE_MAX; ++i ){
		oss.str( "" ); //初始化
		oss << CMAKE_CURRENT_SOURCE_DIR"data/sound/se/" << seFiles[ i ];
		mSeWaves[ i ] = Sound::Wave::create( oss.str().c_str() );
	}
}

SoundManager::~SoundManager(){
}

bool SoundManager::hasLoaded(){
	//检查所有Wave是否都返回true
	//&&运算来说，只要有一个假最终就会变成假。
	bool ret = true;
	for ( int i = 0; i < BGM_MAX; ++i ){
		ret = ret && mBgmWaves[ i ].isReady();
	}
	for ( int i = 0; i < SE_MAX; ++i ){
		ret = ret && mSeWaves[ i ].isReady();
	}
	return ret;
}

void SoundManager::playBgm( Bgm bgm ){
	mBgmPlayer = Sound::Player::create( mBgmWaves[ bgm ] );
	mBgmPlayer.play( true ); //循环播放
}

void SoundManager::stopBgm(){
	if ( mBgmPlayer ){
		mBgmPlayer.stop();
	}
}

void SoundManager::playSe( Se se ){
	mSePlayers[ mSePlayerPos ] = Sound::Player::create( mSeWaves[ se ] );
	mSePlayers[ mSePlayerPos ].play(); //播放
	++mSePlayerPos;
	//
	if ( mSePlayerPos == SE_PLAYER_MAX ){
		mSePlayerPos = 0;
	}
}