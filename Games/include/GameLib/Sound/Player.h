#ifndef INCLUDED_GAMELIB_SOUND_PLAYER_H
#define INCLUDED_GAMELIB_SOUND_PLAYER_H

namespace GameLib{
namespace Sound{

class Wave;

class Player{
public:
	static Player create( Wave );
	///用于播放流的Player
	static Player create( int bitsPerSample, int bufferSizeInByte, int frequency, int channelNumber );

	///播放。如果想无限循环播放，请设置true
	void play( bool looping = false );
	void stop();
	///音量设置（100最大，0最小）
	void setVolume( int volume );
	bool isPlaying() const; //正在播放？

	///写入流缓冲。如果播放位置太近而失败，则返回false
	bool write( int position, const char* data, int size );
	///向流缓冲写入无声。如果播放位置太近而失败，则返回false
	bool fillSilence( int position, int size );
	///以字节单位返回正在播放的缓冲位置
	int position() const;

	//用户不会意识到的函数
	Player();
	Player( const Player& );
	~Player();
	operator void*() const;
	Player& operator=( const Player& );
	void release();
	bool operator==( const Player& ) const;
	bool operator!=( const Player& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Sound
} //namespace GameLib

#endif
