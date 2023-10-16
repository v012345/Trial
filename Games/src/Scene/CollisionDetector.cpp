#include "GameLib/GameLib.h"
#include "GameLib/Scene/CollisionDetector.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/Tank.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Functions.h"

namespace {
using namespace GameLib;
using namespace GameLib::Scene;
typedef CollisionDetector::Pair Pair; //

class Sphere{
public:
	Vector3 mCenter;
	float mRadius;
};

//Node::Build的参数集
class Args{
public:
	Args( 
	const Sphere* spheres,
	int sphereNumber ) : 
	mSpheres( spheres ),
	mIndexPool( sphereNumber ),
	mResults( sphereNumber ){
		mHitFlags = NEW bool[ sphereNumber * 2 ];
	}
	~Args(){
		mSpheres = 0;
		SAFE_DELETE_ARRAY( mHitFlags );
	}

	const Sphere* mSpheres;
	Pool< int > mIndexPool;
	Tank< Pair > mResults;
	bool* mHitFlags;
};

//k-d tree本体
class Node{
public:
	Node() : mIndices( 0 ), mIndexNumber( 0 ){}
	//
	void build(
	const Vector3& minV, //最小范围（左下）
	const Vector3& maxV, //范围最大值（右上方）
	Args* args,
	int restLevel ){
		//如何最佳分割是一个难题，但是在这里我们将分割最长的方向。平均而言，这给出了合理的结果。
		int axis = -1; //分割轴
		Vector3 size;
		size.setSub( maxV, minV );
		float maxSize = -FLOAT_MAX;
		for ( int i = 0; i < 3; ++i ){
			if ( size[ i ] > maxSize ){
				maxSize = size[ i ];
				axis = i;
			}
		}
		//分界线
		float div;
		div = minV[ axis ] + maxV[ axis ];
		div *= 0.5f;
		//判断结果存储在bool数组中，以备后用。
		bool* hitL = args->mHitFlags;
		bool* hitR = hitL + mIndexNumber;
		//左右物体的数量
		int cl = 0;
		int cr = 0;
		//判断其所属
		const Sphere* spheres = args->mSpheres;
		for ( int i = 0; i < mIndexNumber; ++i ){ 
			const Sphere& s = spheres[ mIndices[ i ] ];
			//获取轴的最大最小值
			float minP, maxP;
			minP = s.mCenter[ axis ] - s.mRadius;
			maxP = s.mCenter[ axis ] + s.mRadius;
	
			hitL[ i ] = ( minP < div ); //明らかに左に所属 誤差は考慮しない。動くものの場合誤差でどうこうなっても問題ないし、桁落ちが生じる計算もない。
			hitR[ i ] = ( maxP > div ); //显然属于右侧
			//如果分界线是完美的。
			if ( ( minP == maxP ) && ( minP == div ) ){
				hitR[ i ] = true;
			}
			if ( hitL[ i ] ){
				++cl;
			}
			if ( hitR[ i ] ){
				++cr;
			}
		}
		//开始分割

		//节点是局部变量。不必保存它
		Node left;
		Node right;
		left.mIndices = args->mIndexPool.allocate( cl );
		right.mIndices = args->mIndexPool.allocate( cr );

		for ( int i = 0; i < mIndexNumber; ++i ){ //找出每个三角形
			if ( hitL[ i ] ){
				left.mIndices[ left.mIndexNumber ] = mIndices[ i ];
				++( left.mIndexNumber );
			}
			if ( hitR[ i ] ){
				right.mIndices[ right.mIndexNumber ] = mIndices[ i ];
				++( right.mIndexNumber );
			}
		}
		ASSERT( ( left.mIndexNumber == cl ) && ( right.mIndexNumber == cr ) );

		//
		if ( restLevel > 1 ){ //
			if ( cl > 1 ){
				Vector3 boxDiv = maxV;
				boxDiv[ axis ] = div;
				left.build( minV, boxDiv, args, restLevel - 1 );
			}
			if ( cr > 1 ){
				Vector3 boxDiv = minV;
				boxDiv[ axis ] = div;
				right.build( boxDiv, maxV, args, restLevel - 1 );
			}
		}else{
			//来判断。
			left.test( args );
			right.test( args );
		}
	}
	void test( Args* args ) const {
		const Sphere* spheres = args->mSpheres;

		for ( int i = 0; i < mIndexNumber; ++i ){
			int i0 = mIndices[ i ];
			const Sphere& s0 = spheres[ mIndices[ i ] ];
			const Vector3& p0 = s0.mCenter;
			float r0 = s0.mRadius;
			for ( int j = i + 1; j < mIndexNumber; ++j ){
				int i1 = mIndices[ j ];
				const Sphere& s1 = spheres[ mIndices[ j ] ];
				const Vector3& p1 = s1.mCenter;
				float r1 = s1.mRadius;
				//球的判断。不用担心误差
				Vector3 d;
				d.setSub( p1, p0 );
				float rsum = r0 + r1;
				float rsum2 = rsum * rsum;
				float dist2 = d.squareLength();
				if ( dist2 <= rsum2 ){
					Pair* pair = args->mResults.add();
					if ( i0 < i1 ){ //较小的是mId0。
						pair->mId0 = i0;
						pair->mId1 = i1;
					}else{
						pair->mId0 = i1;
						pair->mId1 = i0;
					}
				}
			}
		}
	}
	int* mIndices;
	int mIndexNumber;
};

} //namespace {}

namespace GameLib{
namespace Scene{

class CollisionDetector::Impl : public ReferenceType{
public:
	Impl( int capacity, int maxLevel ) : 
	mSpheres( 0 ),
	mSphereNumber( 0 ),
	mCapacity( capacity ),
	mMaxLevel( maxLevel ){
		mSpheres = NEW Sphere[ mCapacity ];
	}
	~Impl(){
		SAFE_DELETE_ARRAY( mSpheres );
	}
	int add( const Vector3& c, float r ){
		int i = mSphereNumber;
		ASSERT( i < mCapacity );
		mSpheres[ i ].mCenter = c;
		mSpheres[ i ].mRadius = r;
		++mSphereNumber;
		return i;
	}
	void detect( Array< Pair >* resultsOut ){
#if 1
		Node root;
		Args args( mSpheres, mSphereNumber );
		root.mIndexNumber = mSphereNumber;
		root.mIndices = args.mIndexPool.allocate( mSphereNumber );
		//用数据填充根节点。同时计算范围
		Vector3 minV( FLOAT_MAX ); //最大初始化
		Vector3 maxV( -minV.x ); //最小初始化
		for ( int i = 0; i < mSphereNumber; ++i ){
			const Sphere& s = mSpheres[ i ];
			Vector3 tMin = s.mCenter;
			Vector3 tMax = s.mCenter;
			Vector3 r( s.mRadius );
			tMin -= r;
			tMin += r;
			minV.min( tMin ); //min取XYZ中的最小值
			maxV.max( tMax ); //max以XYZ取最大值。
			//
			root.mIndices[ i ] = i;
		}
		//已为框中的每个球准备了节点，但以后可能会更改。
		//必需找到至少为n的2的最小幂，并准备两倍的最小幂。
		int n = mSphereNumber;
		int maxNodeNumber = 1;
		int maxLevel = 0; //最大分割数
		while ( maxNodeNumber < n ){
			maxNodeNumber <<= 1;
			++maxLevel;
		}
		root.build( minV, maxV, &args, maxLevel );
		//删除Tank中的重复项。
		//1.转到数组以加速访问
		int resultSize = args.mResults.size();
		Pair* resultsArray = NEW Pair[ resultSize ];
		int pos = 0;
		while ( !args.mResults.isEnd() ){
			resultsArray[ pos ] = *args.mResults.get();
			++pos;
			args.mResults.toNext();
		}
		//2.数第一个是i的有几个。
		int* counts = NEW int[ mSphereNumber * 2 ];
		int* offsets = counts + mSphereNumber; //固定在一起。这样做可能会导致错误
		for ( int i = 0; i < mSphereNumber; ++i ){ //初始化循环
			counts[ i ] = 0;
		}
		for ( int i = 0; i < resultSize; ++i ){
			++counts[ resultsArray[ i ].mId0 ];
		}
		//2.转换为偏移
		int offset = 0;
		for ( int i = 0; i < mSphereNumber; ++i ){
			offsets[ i ] = offset;
			offset += counts[ i ];
			counts[ i ] = 0;
		}
		//1.同时保存排序后的数组
		int* sorted = NEW int[ offset ]; //偏移量仅包含总和。
		for ( int i = 0; i < resultSize; ++i ){
			int i0 = resultsArray[ i ].mId0;
			sorted[ offsets[ i0 ] + counts[ i0 ] ] = resultsArray[ i ].mId1;
			++counts[ i0 ];
		}
		//
		resultsOut->setSize( offset ); //
		pos = 0;
		for ( int i = 0; i < mSphereNumber; ++i ){
			int* box = &sorted[ offsets[ i ] ];
			int n = counts[ i ];
			for ( int j = 0; j < n; ++j ){
				if ( box[ j ] >= 0 ){
					( *resultsOut )[ pos ].mId0 = i;
					( *resultsOut )[ pos ].mId1 = box[ j ];
					++pos;
					for ( int k = j + 1; k < n; ++k ){ //如果后面有同一个，将其销毁。
						if ( box[ j ] == box[ k ] ){
							box[ k ] = -1;
						}
					}
				}
			}
		}
		//减小最终结果的大小。不会发生任何移动。
		resultsOut->setSize( pos );
		//清理工作
		SAFE_DELETE_ARRAY( sorted );
		SAFE_DELETE_ARRAY( counts );
		SAFE_DELETE_ARRAY( resultsArray );
#else //
		Tank< Pair > results( mSphereNumber );
		for ( int i = 0; i < mSphereNumber; ++i ){
			const Sphere& s0 = mSpheres[ i ];
			const Vector3& p0 = s0.mCenter;
			float r0 = s0.mRadius;
			for ( int j = i + 1; j < mSphereNumber; ++j ){
				const Sphere& s1 = mSpheres[ j ];
				const Vector3& p1 = s1.mCenter;
				float r1 = s1.mRadius;
				//球的判断。稍后再考虑误差
				Vector3 d;
				d.setSub( p1, p0 );
				float rsum = r0 + r1;
				float rsum2 = rsum * rsum;
				float dist2 = d.getSquareLength();
				if ( dist2 <= rsum2 ){
					Pair* pair = results.add();
					pair->mId0 = i;
					pair->mId1 = j;
				}
			}
		}
		//转换为数组
		results.copyTo( resultsOut );
#endif
		//判断后，球数设为0。
		mSphereNumber = 0;
	}
private:
	void operator=( const Impl& ); //禁止

	Sphere* mSpheres;
	int mSphereNumber;
	const int mCapacity;
	const int mMaxLevel;
};

CollisionDetector::CollisionDetector( Impl* impl ) : mImpl( impl ){
	if ( mImpl ){
		mImpl->refer();
	}
}

CollisionDetector CollisionDetector::create( int capacity, int maxLevel ){
	CollisionDetector r;
	r.mImpl = NEW Impl( capacity, maxLevel );
	return r;
}

int CollisionDetector::add( const Vector3& sphereCenter, float sphereRadius ){
	return mImpl->add( sphereCenter, sphereRadius );
}

void CollisionDetector::detect( Array< Pair >* resultOut ){
	mImpl->detect( resultOut );
}

#define TYPE CollisionDetector
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

} //namespace Scene
} //namespace GameLib
