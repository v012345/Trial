#ifndef INCLUDED_GAMELIB_MATH_RANDOM_H
#define INCLUDED_GAMELIB_MATH_RANDOM_H

namespace GameLib{
namespace Math{

///随机数生成类
class Random{
public:
	static Random create( int seed = -1 );
	///重新设置种子
	void setSeed( int );
	///-0x80000000 - 0x7fffffff
	int getInt();
	//至a-1为止[0，a）
	int getInt( int a );
	//从a到b-1为止[a，b）
	int getInt( int a, int b );
	///0-1但是不包含1（23比特）
	float getFloat();
	//从a到b。但是不包含b。[a，b）（23比特）
	float getFloat( float a );
	//从a到b。但是不包含b。[a，b）（23比特）
	float getFloat( float a, float b );

	//你不需要知道
	Random();
	Random( const Random& );
	~Random();
	operator void*() const;
	Random& operator=( const Random& );
	void release();
	bool operator==( const Random& ) const;
	bool operator!=( const Random& ) const;
	bool operator<( const Random& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Math
} //namespace GameLib

#endif