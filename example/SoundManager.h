#ifndef INCLUDED_SOUND_MANAGER_H
#define INCLUDED_SOUND_MANAGER_H

#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"

//单例
class SoundManager{
public:
	static SoundManager* instance();
	static void create();
	static void destroy();
	bool hasLoaded(); //完成加载？

	enum Bgm{
		BGM_TITLE, //主题画面
		BGM_GAME, //在游戏中

		BGM_MAX,
	};
	enum Se{
		SE_EXPLOSION, //爆炸
		SE_DEATH, //死亡
		SE_CURSOR_MOVE, //光标移动
		SE_SELECTION, //项目决定
		SE_SET_BOMB, //炸弹放置声

		SE_MAX,
	};
	//播放BGM。一次只发出一个BGM声音。另外还会循环。
	void playBgm( Bgm );
	void stopBgm();
	//SE播放音效。只有另其播放时才会发声。
	void playSe( Se );
private:
	SoundManager();
	~SoundManager();

	//预先施加Wave。
	GameLib::Sound::Wave mBgmWaves[ BGM_MAX ];
	GameLib::Sound::Wave mSeWaves[ SE_MAX ];
	//在播放时生成Player
	GameLib::Sound::Player mBgmPlayer; //只做一个
	static const int SE_PLAYER_MAX = 4; //多达四个SE同时发出声音。我们按旧顺序删除。
	GameLib::Sound::Player mSePlayers[ SE_PLAYER_MAX ]; //检查数组中的每帧，结束后将其删除。
	int mSePlayerPos; //放置SE的地方。

	static SoundManager* mInstance;
};

#endif
