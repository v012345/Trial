#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

class StaticObject{
public:
	enum Flag{
		FLAG_WALL = ( 1 << 0 ), //具体
		FLAG_BRICK = ( 1 << 1 ), //
		FLAG_BOMB = ( 1 << 2 ), //炸弹
		FLAG_ITEM_BOMB = ( 1 << 3 ), //炸弹道具
		FLAG_ITEM_POWER = ( 1 << 4 ), //冲击波道具
		FLAG_FIRE_X = ( 1 << 5 ), //横向火焰
		FLAG_FIRE_Y = ( 1 << 6 ), //纵向火焰
		FLAG_EXPLODING = ( 1 << 7 ), //爆炸
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//绘制地板，墙壁，砖块，炸弹和物品（如果有）
	void draw( int x, int y, const Image* ) const;
	//绘制爆炸冲击波
	void drawExplosion( int x, int y, const Image* ) const ;

	int mCount;	//计数（炸弹安装，爆炸，开始燃烧）
	//仅炸弹
	DynamicObject* mBombOwner; //炸弹主人
private:
	unsigned mFlags; //标志
};

#endif
