#ifndef INCLUDED_GAMELIB_THREADING_SEMAPHORE_H
#define INCLUDED_GAMELIB_THREADING_SEMAPHORE_H

namespace GameLib{
namespace Threading{

///可以按指定人数同时加锁的Mutex。
class Semaphore{
public:
	///根据参数的初始值生成。maxCount是0的话就是startCount。
	static Semaphore create( int startCount, int maxCount = 0 );
	///减少一个计数。已经0的话会休眠。
	void decrease();
	///增加计数。参数省略表示一个个进行。
	void increase( int value = 1 );

	Semaphore();
	~Semaphore();
	Semaphore( const Semaphore& );
	operator void*() const;
	Semaphore& operator=( const Semaphore& );
	void release();
	bool operator==( const Semaphore& ) const;
	bool operator!=( const Semaphore& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Threading
} //namespace GameLib

#endif
