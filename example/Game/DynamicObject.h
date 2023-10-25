#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;
class StaticObject;

class DynamicObject{
public:
	enum Type{
		TYPE_PLAYER,
		TYPE_ENEMY,

		TYPE_NONE, //死了
	};
	DynamicObject();
	void set( int x, int y, Type );
	void draw( const Image* ) const;
	//移动
	void move( const int* wallsX, int* wallsY, int wallNumber );
	//查询哪个网格中心。
	void getCell( int* x, int* y ) const;
	//返回在数组中被dx和dy移动时重叠的质量坐标。
	//参数是两个int [4]。返回值是输入的数字。1,2,4之一。
	int getIntersectionCell( int* xArray, int* yArray, int dx, int dy ) const;
	//
	void doCollisionReactionToStatic( StaticObject** o, int cellNumber );
	void doCollisionReactionToDynamic( DynamicObject* );

	//与DynamicObject相交的判断
	bool isIntersect( const DynamicObject& ) const;
	//墙壁
	bool isIntersectWall( int wallCellX, int wallCellY );

	//便利函数
	bool hasBombButtonPressed() const;	//找出是否按下了炸弹按钮
	bool isPlayer() const;
	bool isEnemy() const;
	void die(); //死亡（将mType设置为NONE）
	bool isDead() const; //死了吗

	//不要将直接修改的内容字段设为私有。因为会经常从State使用它。
	Type mType;
	//玩家专用
	int mBombPower; //爆炸力
	int mBombNumber; //炸弹数量
	int mPlayerID; //玩家编号
	//只给那些放炸弹的人
	int mLastBombX[ 2 ];
	int mLastBombY[ 2 ];
private:
	//墙壁
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	//获取当前帧的移动量
	void getVelocity( int* dx, int* dy ) const;
	//取得方向
	void getDirection( int* dx, int* dy ) const;
	//坐标（内部单位）
	int mX;
	int mY;
	//仅限敌人
	int mDirectionX;
	int mDirectionY;
};

#endif
