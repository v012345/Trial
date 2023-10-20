#ifndef INCLUDED_SEQUENCE_GAME_LOADING_H
#define INCLUDED_SEQUENCE_GAME_LOADING_H

class Image;

namespace Sequence{
namespace Game{
class Parent;

class Loading{
public:
	Loading();
	~Loading();
	void update( Parent* );
private:
	Image* mImage;
	int mCount;
	bool mStarted; //是否已发出加载启动请求？
};

} //namespace Game
} //namespace Sequence

#endif