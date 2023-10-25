#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H

#include "Array2D.h"

class Image; //只需要声明。不必包含它。
class StaticObject;
class DynamicObject;

class State{
public:
	//给出阶段编号并构建。0是两个人
	static const int STAGE_ID_2PLAYERS = 0;
	State( int stageID );
	~State();
	void update();
	void draw() const;
	//从外部获取的信息
	bool hasCleared() const;
	bool isAlive( int playerID ) const; //还活着吗
private:
	//火焰设置。坐标是炸弹坐标
	void setFire( int x, int y );

	//不动的物体
	Array2D< StaticObject > mStaticObjects;
	//运动物体
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;

	int mStageID;

	Image* mImage; //图片
};

#endif
