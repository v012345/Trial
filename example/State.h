#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image; //只需要声明。不必包含它。

//状态类
class State{
public:
	State( const char* stageData, int size );
	~State();
	void update( char input );
	void draw() const;
	bool hasCleared() const;
private:
	enum Object{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	//网格绘制函数
	enum ImageID{
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};
	void setSize( const char* stageData, int size );
	void drawCell( int x, int y, ImageID ) const;

	int mWidth;
	int mHeight;
	Array2D< Object > mObjects;
	Array2D< bool > mGoalFlags;
	Image* mImage; //图片
};

#endif
