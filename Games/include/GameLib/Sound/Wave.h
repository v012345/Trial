#ifndef INCLUDED_GAMELIB_SOUND_WAVE_H
#define INCLUDED_GAMELIB_SOUND_WAVE_H

namespace GameLib{
namespace Sound{

class Wave{
public:
	static Wave create( const char* filename );
	///波形输入（8比特）
	static Wave create( const unsigned char* data, int size, int frequency, int channelNumber ); 
	///波形输入（16比特）
	static Wave create( const short* data, int size, int frequency, int channelNumber );
	bool isReady(); //完成加载？
	bool isError() const; //发生错误了吗？

	//用户不会意识到的函数
	Wave();
	Wave( const Wave& );
	~Wave();
	operator void*() const;
	Wave& operator=( const Wave& );
	void release();
	bool operator==( const Wave& ) const;
	bool operator!=( const Wave& ) const;
private:
	class Impl;
	friend class Player;
	Impl* mImpl;
};

} //namespace Sound
} //namespace GameLib

#endif
