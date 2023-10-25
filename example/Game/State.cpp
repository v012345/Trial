#include "Games/Framework.h"
#include "GameLib/Gamelib.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"

#include "Image.h"

using namespace GameLib;

namespace {

//地图尺寸
static const int WIDTH = 19;
static const int HEIGHT = 15;
//炸弹参数
static const int EXPLOSION_TIME = 180; //3秒数
static const int EXPLOSION_LIFE = 60; //1秒数

//合适的舞台数据
struct StageData{
	int mEnemyNumber; //敌人数
	int mBrickRate; //砖比率（百分比）
	int mItemPowerNumber; //冲击波道具数量
	int mItemBombNumber; //炸弹数量
};

static StageData gStageData[] = {
	{ 2, 50, 10, 10, },
	{ 3, 70, 1, 0, },
	{ 5, 30, 0, 1, },
};

} //namespace{}

State::State( int stageID ) : 
mImage( 0 ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStageID( stageID ){
	Framework f = Framework::instance(); //再用几次
	mStaticObjects.setSize( WIDTH, HEIGHT );

	mImage = new Image( CMAKE_CURRENT_SOURCE_DIR"data/image/bakudanBitoImage.dds" );

	const StageData& stageData = gStageData[ mStageID ];
	int n = HEIGHT * WIDTH; //网格数

	//记录砖块。
	unsigned* brickList = new unsigned[ n ];
	int brickNumber = 0; //计算真正的砖块数量
	unsigned* floorList = new unsigned[ n ];
	int floorNumber = 0;

	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( x == 0 || y == 0 || ( x == WIDTH-1 ) || ( y == HEIGHT-1 ) ){
				o.setFlag( StaticObject::FLAG_WALL );
			}else if ( ( x % 2 == 0 ) && ( y % 2 == 0 ) ){ //具体的
				o.setFlag( StaticObject::FLAG_WALL );
			}else if ( y + x < 4 ){
				//前三个网格是地面
			}else if ( ( stageID == 0 ) && ( y + x > ( WIDTH + HEIGHT - 6 ) ) ){
				//如果有两个人，请在右下角留三个网格
			}else{ //其余的是砖或地板。随机决定
				if ( f.getRandom( 100 ) < stageData.mBrickRate  ){
					o.setFlag( StaticObject::FLAG_BRICK );
					//如果是砖头，请记录下来。
					brickList[ brickNumber ] = ( x << 16 ) + y;
					++brickNumber;
				}else{
					floorList[ floorNumber ] = ( x << 16 ) + y;
					++floorNumber;
				}
			}
		}
	}
	//将道具放在砖中
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;
	//这样做的方法是将砖列表中的第i个项目替换为合适的项目，然后将其放置在此处。
	for ( int i = 0; i < powerNumber + bombNumber; ++i ){
 		int swapped = f.getRandom( brickNumber - 1 - i ) + i; //
		unsigned t = brickList[ i ];
		brickList[ i ] = brickList[ swapped ];
		brickList[ swapped ] = t;

		int x = brickList[ i ] >> 16;
		int y = brickList[ i ] & 0xffff;
		StaticObject& o = mStaticObjects( x, y );
		if ( i < powerNumber ){
			o.setFlag( StaticObject::FLAG_ITEM_POWER );
		}else{
			o.setFlag( StaticObject::FLAG_ITEM_BOMB );
		}
	}
	SAFE_DELETE_ARRAY( brickList );

	//分配动态对象
	int playerNumber = ( mStageID == 0 ) ? 2 : 1;
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[ mDynamicObjectNumber ];

	//玩家位置
	mDynamicObjects[ 0 ].set( 1, 1, DynamicObject::TYPE_PLAYER );
	mDynamicObjects[ 0 ].mPlayerID = 0;
	if ( mStageID == 0 ){
		mDynamicObjects[ 1 ].set( WIDTH-2, HEIGHT-2, DynamicObject::TYPE_PLAYER );
		mDynamicObjects[ 1 ].mPlayerID = 1;
	}
	//将敌人放到地板上。方式与放置道具几乎相同
	for ( int i = 0; i < enemyNumber; ++i ){
 		int swapped = f.getRandom( floorNumber - 1 - i ) + i;
		unsigned t = floorList[ i ];
		floorList[ i ] = floorList[ swapped ];
		floorList[ swapped ] = t;

		int x = floorList[ i ] >> 16;
		int y = floorList[ i ] & 0xffff;
		mDynamicObjects[ playerNumber + i ].set( x, y, DynamicObject::TYPE_ENEMY );
	}
	SAFE_DELETE_ARRAY( floorList );
}

State::~State(){
	SAFE_DELETE( mImage );
	SAFE_DELETE_ARRAY( mDynamicObjects );
}

void State::draw() const {
	//绘制背景
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).draw( x, y, mImage );
		}
	}
	//绘制前景
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].draw( mImage );
	}
	//绘制冲击波
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).drawExplosion( x, y, mImage );
		}
	}
}

void State::update(){
	//炸弹处理
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( o.checkFlag( StaticObject::FLAG_BOMB ) ){
				//1.更新炸弹计数
				++o.mCount;
				//2.爆破开始、结束判定
				if ( o.checkFlag( StaticObject::FLAG_EXPLODING ) ){ //灭火判断
					if ( o.mCount == EXPLOSION_LIFE ){ //爆炸结束时间
						o.resetFlag( StaticObject::FLAG_EXPLODING | StaticObject::FLAG_BOMB );
						o.mCount = 0;
					}
				}else{ //爆炸判断
					if ( o.mCount == EXPLOSION_TIME ){ //到了爆炸时刻
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
					}else if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //爆炸
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
					}
				}
			}else if ( o.checkFlag( StaticObject::FLAG_BRICK ) ){ //对于砖，需要判断烧尽
				if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //
					++o.mCount; //
					if ( o.mCount == EXPLOSION_LIFE ){
						o.mCount = 0;
						o.resetFlag( StaticObject::FLAG_BRICK ); //烧毁了
					}
				}
			}
			//3.因为爆炸气浪每架重新放置，所以关掉一次。
			o.resetFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y );
		}
	}
	//火焰设定
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			if ( mStaticObjects( x, y ).checkFlag( StaticObject::FLAG_EXPLODING ) ){
				setFire( x, y );
			}
		}
	}

	//计算1P、2P的设置炸弹数量
	int bombNumber[ 2 ];
	bombNumber[ 0 ] = bombNumber[ 1 ] = 0;
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			const StaticObject& o = mStaticObjects( x, y );
			if ( o.checkFlag( StaticObject::FLAG_BOMB ) ){
				++bombNumber[ o.mBombOwner->mPlayerID ];
			}
		}
	}
	//与动态对象循环
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		DynamicObject& o = mDynamicObjects[ i ];
		if ( o.isDead() ){ //死了 结束
			continue;
		}
		//检查它是否与放置的炸弹接触
		for ( int j = 0; j < 2; ++j ){
			if ( o.mLastBombX[ j ] >= 0 ){ //0以上的话里面有什么。
				if ( !o.isIntersectWall( o.mLastBombX[ j ], o.mLastBombY[ j ] ) ){
					o.mLastBombX[ j ] = o.mLastBombY[ j ] = -1;
				}
			}
		}
		//获取当前单元格
		int x, y;
		o.getCell( &x, &y );
		//从以此为中心的单元中找到墙并将其存储在数组中
		int wallsX[ 9 ];
		int wallsY[ 9 ];
		int wallNumber = 0;
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				int tx = x + i - 1;
				int ty = y + j - 1;
				const StaticObject& so = mStaticObjects( tx, ty );
				if ( so.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //在墙上
					bool myBomb0 = ( o.mLastBombX[ 0 ] == tx ) && ( o.mLastBombY[ 0 ] == ty );
					bool myBomb1 = ( o.mLastBombX[ 1 ] == tx ) && ( o.mLastBombY[ 1 ] == ty );
					if ( !myBomb0 && !myBomb1 ){ //不是自己放的炸弹
						wallsX[ wallNumber ] = x + i - 1;
						wallsY[ wallNumber ] = y + j - 1;
						++wallNumber;
					}
				}
			}
		}
		//
		o.move( wallsX, wallsY, wallNumber );
		//通过判断与移​​动后的位置的9个周围网格的碰撞来进行各种反应
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				StaticObject& so = mStaticObjects( x + i - 1, y + j - 1 );
				if ( o.isIntersectWall( x + i - 1, y + j - 1 ) ){ //接触了
					if ( so.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){
						o.die(); //烧了
					}else if ( !so.checkFlag( StaticObject::FLAG_BRICK ) ){ //如果发现项目
						if ( so.checkFlag( StaticObject::FLAG_ITEM_POWER ) ){
							so.resetFlag( StaticObject::FLAG_ITEM_POWER );
							++o.mBombPower;
						}else if ( so.checkFlag( StaticObject::FLAG_ITEM_BOMB ) ){
							so.resetFlag( StaticObject::FLAG_ITEM_BOMB );
							++o.mBombNumber;
						}
					}
				}
			}
		}
		//移动后获取网格编号
		o.getCell( &x, &y );
		//放炸弹
		if ( o.hasBombButtonPressed() ){ //炸弹放置按钮已被按下
			if ( bombNumber[ o.mPlayerID ] < o.mBombNumber ){ //低于炸弹最大值
				StaticObject& so = mStaticObjects( x, y );
				if ( !so.checkFlag( StaticObject::FLAG_BOMB ) ){ //没有炸弹
					so.setFlag( StaticObject::FLAG_BOMB );
					so.mBombOwner = &o;
					so.mCount = 0;

					//更新炸弹位置
					if ( o.mLastBombX[ 0 ] < 0 ){
						o.mLastBombX[ 0 ] = x;
						o.mLastBombY[ 0 ] = y;
					}else{
						o.mLastBombX[ 1 ] = x;
						o.mLastBombY[ 1 ] = y;
					}
				}
			}
		}
	}
	//下一个 判断敌人与玩家之间的碰撞。
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		for ( int j = i + 1; j < mDynamicObjectNumber; ++j ){
			mDynamicObjects[ i ].doCollisionReactionToDynamic( &mDynamicObjects[ j ] );
		}
	}
}

void State::setFire( int x, int y ){
	StaticObject& o = mStaticObjects( x, y );
	int power = o.mBombOwner->mBombPower;
	int end;
	
	//循环中的处理在4个循环中相同。
	//如果想这样做，可以做到。但是，其缺点是创建的功能较难理解
	//这里写了四次。

	//向左
	end = ( x - power < 0 ) ? 0 : ( x - power );
	for ( int i = x - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( i, y );
		to.setFlag( StaticObject::FLAG_FIRE_X );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //
			break;
		}else{
			//如果有任何物品，则销毁
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//右
	end = ( x + power >= WIDTH ) ? ( WIDTH - 1 ) : ( x + power );
	for ( int i = x + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( i, y );
		to.setFlag( StaticObject::FLAG_FIRE_X );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //
			break;
		}else{
			//如果有任何物品，则销毁
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//上
	end = ( y - power < 0 ) ? 0 : ( y - power );
	for ( int i = y - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( x, i );
		to.setFlag( StaticObject::FLAG_FIRE_Y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //
			break;
		}else{
			//如果有任何物品，则销毁
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//下
	end = ( y + power >= HEIGHT ) ? ( HEIGHT - 1 ) : ( y + power );
	for ( int i = y + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( x, i );
		to.setFlag( StaticObject::FLAG_FIRE_Y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //
			break;
		}else{
			//如果有任何物品，则销毁
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}

	//[下面内容比较难理解]

	//从这往下
	//
	//○○□□
	//○是炸弹，□是砖块。首先从右侧爆炸
	//○●■□
	// ●爆炸，■烧坏的砖块。之后，砖块被炸毁
	//●　　□
	//中央的两个消失了。无法阻挡左边的爆炸
	//●→→■
	//它会被烧掉。为了防止这种情况，必须将砖块处理放在最后
	//为此，有必要将新爆炸的爆炸范围内的砖块数初始化为零。
	//有很多方法可以做到这一点，但采用了这种方法，只需要粘贴一半即可。
	//实际上，在爆炸开始时固定爆炸物会更好。
	//
	//但是因为这个改造比较大，所以它可能不是一个很好的示例

	//向左
	end = ( x - power < 0 ) ? 0 : ( x - power );
	for ( int i = x - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( i, y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ //
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //开始计算砖的烧尽
				to.mCount = 0;
			}
			break;
		}
	}
	//右
	end = ( x + power >= WIDTH ) ? ( WIDTH - 1 ) : ( x + power );
	for ( int i = x + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( i, y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ 
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //开始计算砖的烧尽
				to.mCount = 0;
			}
			break;
		}
	}
	//上
	end = ( y - power < 0 ) ? 0 : ( y - power );
	for ( int i = y - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( x, i );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ 
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //开始计算砖的烧尽
				to.mCount = 0;
			}
			break;
		}
	}
	//下
	end = ( y + power >= HEIGHT ) ? ( HEIGHT - 1 ) : ( y + power );
	for ( int i = y + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( x, i );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //开始计算砖的烧尽
				to.mCount = 0;
			}
			break;
		}
	}
}

bool State::hasCleared() const {
	//清除是否没有敌人
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		if ( mDynamicObjects[ i ].isEnemy() ){
			return false;
		}
	}
	return true;
}

bool State::isAlive( int playerID ) const {
	//如果还活着
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		if ( mDynamicObjects[ i ].mType == DynamicObject::TYPE_PLAYER ){
			if ( mDynamicObjects[ i ].mPlayerID == playerID ){
				return true;
			}
		}
	}
	return false;
}

