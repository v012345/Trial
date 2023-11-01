#ifndef INCLUDED_SEQUENCE_STAGE_SELECT_H
#define INCLUDED_SEQUENCE_STAGE_SELECT_H

class Image;

namespace Sequence{
class Parent;

class StageSelect{
public:
	StageSelect();
	~StageSelect();
	void update( Parent* );
private:
	Image* mImage; //
	int mCursorPosition; //选择光标位置
};

} //namespace Sequence

#endif