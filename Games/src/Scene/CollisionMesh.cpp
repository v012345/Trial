#include "GameLib/GameLib.h"
#include "GameLib/Scene/CollisionMesh.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Functions.h"
//using namespace std;
using namespace GameLib;
using namespace GameLib::PseudoXml;
using namespace GameLib::Math;

//define STRONG DEBUG//每次确认这一行是否有效，与k-d tree的总结果是否完全一致。
int G;

namespace{

//解决方程组的函数。通过克莱姆方法求解。
//| ax bx cx | | ox | = | dx |
//| ay by cy | | oy | = | dy |
//| az bz cz | | oz | = | dz |
//取四个3D向量，返回一个解。o是求出的解。
//但是，为了提高效率，每次有解时都会做出判断，如果超出范围，则立即返回false
//仅在碰撞中时返回true。
bool solveLinearSystem(
Vector3* o,
const Vector3& a,
const Vector3& b,
const Vector3& c,
const Vector3& d ){
	float m1021 = a.y * b.z;
	float m1022 = a.y * c.z;

	float m1120 = b.y * a.z;
	float m1122 = b.y * c.z;

	float m1220 = c.y * a.z;
	float m1221 = c.y * b.z;

	//2三个项积之差。虽然是行列式用的，但之后让他挪用。
	float m1122_m1221 = m1122 - m1221;
	float m1220_m1022 = m1220 - m1022; //负
	float m1021_m1120 = m1021 - m1120;

	//行列式
	//00*( 11*22 - 12*21 ) - 01*( 10*22 - 12*20 ) + 02*( 10*21 - 11*20 )
	float delta = a.x*( m1122_m1221 ) + b.x*( m1220_m1022 ) + c.x*( m1021_m1120 );
	if ( delta == 0.f ){ //
		return false;
	}
	float rcpDelta = 1.f / delta;	
	
	//计算误差的问题非常麻烦。
	//此处，考虑到位数损失限制为三位数，因此使用EPSILON * 1000。
	//
	static const float zero = 0.f - EPSILON * 1000.f;
	static const float one = 1.f + EPSILON * 1000.f;

	//通过将所得逆矩阵乘以右侧向量来计算解。最后除以delta
	float m0221 = c.x * b.z;
	float m0122 = b.x * c.z;
	float m0112 = b.x * c.y;
	float m0211 = c.x * b.y;
	o->x = m1122_m1221 * d.x + ( m0221-m0122 ) * d.y + ( m0112-m0211 ) * d.z;
	//判断t。
	o->x *= rcpDelta;
	if ( ( o->x < zero ) || ( o->x > one ) ){
		return false;
	}
	//接下来计算u
	float m0022 = a.x * c.z;
	float m0220 = c.x * a.z;
	float m0210 = c.x * a.y;
	float m0012 = a.x * c.y;
	o->y = m1220_m1022 * d.x + ( m0022-m0220 ) * d.y + ( m0210-m0012 ) * d.z;
	o->y *= rcpDelta;
	if ( o->y < zero ){
		return false;
	}
	//最后计算v
	float m0120 = b.x * a.z;
	float m0021 = a.x * b.z;
	float m0011 = a.x * b.y;
	float m0110 = b.x * a.y;
	o->z = m1021_m1120 * d.x + ( m0120-m0021 ) * d.y + ( m0011-m0110 ) * d.z;
	o->z *= rcpDelta;
	if ( o->z < zero ){
		return false;
	}
	//检查u和v的总和是否小于等于1
	if ( ( o->y + o->z ) > one ){
		return false;
	}
	return true;
}

struct Triangle{
	int mIndices[ 3 ];
};

//请求碰撞处理时要传什么。减少参数并减少开销的巧妙方法
struct Query{
	Query( 
	const Vector3& begin,
	const Vector3& vec,
	const Triangle* triangles,
	const Vector3* vertices ) :
	mBegin( begin ), mVector( vec ), mTriangles( triangles ), mVertices( vertices ){
	}
	~Query(){
		mTriangles = 0;
		mVertices = 0;
	}
	const Vector3& mBegin;
	const Vector3& mVector;
	const Triangle* mTriangles;
	const Vector3* mVertices;
private:
	void operator=( const Query& ); //没有
};

//为了构建节点时减少传递给build（）的参数而使用的结构体
class Node;
struct BuildArgs{
	BuildArgs( 
	const Triangle* triangles,
	const Vector3* vertices,
	Node* nodes,
	int triangleNumber ) :
	mTriangles( triangles ),
	mVertices( vertices ),
	mNodePos( nodes ),
	mIndexPool( triangleNumber ),
	mHitFlags( 0 ){
		mHitFlags = NEW bool[ triangleNumber * 2 ];
	}
	~BuildArgs(){
		SAFE_DELETE_ARRAY( mHitFlags );
		mTriangles = 0;
		mVertices = 0;
		mNodePos = 0;
	}

	const Triangle* mTriangles;
	const Vector3* mVertices;
	Node* mNodePos; //节点数组指针
	Pool< int > mIndexPool; //用于分配索引的池
	bool* mHitFlags; //
private:
	void operator=( const BuildArgs& ); //没有
};

bool getIntersectionTriangleAndLineSegment( 
float* tOut, 
const Query& q,
int index ){ //
	const Triangle& tri = q.mTriangles[ index ];
	int i0 = tri.mIndices[ 0 ];
	int i1 = tri.mIndices[ 1 ];
	int i2 = tri.mIndices[ 2 ];
	const Vector3& p0 = q.mVertices[ i0 ];
	const Vector3& p1 = q.mVertices[ i1 ];
	const Vector3& p2 = q.mVertices[ i2 ];

	Vector3 minusD, minusE; //
	Vector3 f;
	f.setSub( p0, q.mBegin ); 
	minusD.setSub( p0, p1 );
	minusE.setSub( p0, p2 );
	//这里对联立方程式求解。见正文18.3.2。
	//| bx -dx -ex | | ox | = | fx |
	//| by -dy -ey | | oy | = | fy |
	//| bz -dz -ez | | oz | = | fz |
	//只要计算bt+du+ev=p0-a即可
	Vector3 tuv; //解
	if ( solveLinearSystem( &tuv, q.mVector, minusD, minusE, f ) ){
		*tOut = tuv.x; //返回t
		return true;
	}else{
		return false; //
	}
}

class Node{
public:
	struct InnerNode{ //内部节点成员
		float mLine; //分界线
		Node* mLeft; //
		Node* mRight; //面
	};
	struct OuterNode{ //终端节点的成员。只有末端节点具有三角形。
		int* mIndices; //三角形编号数组
		int mIndexNumber;
	};
	Node() : mAxis( -1 ){
		mInner.mLine = 0.f;
		mInner.mLeft = mInner.mRight = 0;
		mOuter.mIndices = 0;
		mOuter.mIndexNumber = 0;
	}
	//递归判断函数
	bool isIntersect( 
	const Query& q,
	float beginT, //a和b的值保持不变，并且箱子的大小由a'=a+beginT*b,b'=a+endT*b算出。
	float endT ) const {
		//
		if ( mAxis == -1 ){ //如果分割轴为-1
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				int triIdx = mOuter.mIndices[ i ];
++G;
				float dummyT;
				if ( getIntersectionTriangleAndLineSegment( &dummyT, q, triIdx ) ){ //判断原始线段。
					return true;
				}
			}
		}else{ //
			//别名
			const Vector3& a = q.mBegin;
			const Vector3& b = q.mVector;
			const Node* l = mInner.mLeft;
			const Node* r = mInner.mRight;
			//计算和平面相交的时刻。从d = a + bt得到 t =（d-a）/ b
			float divT;
			if ( b[ mAxis ] == 0.f ){ //分母0
				divT = FLOAT_MAX;
			}else{
				divT = ( mInner.mLine - a[ mAxis ] ) / b[ mAxis ];
			}
			float endT0, beginT1; //第一个节点的结束时间，下一个节点的开始时间
			if ( divT >= endT ){ //不到达下一个节点
				endT0 = endT;
				beginT1 = endT;
			}else if ( divT > beginT ){ //赶到下一个节点
				//如果没有计算错误，请照原样传递divT，
				endT0 = ( endT + divT * 15.f ) * ( 1.f / 16.f );
				beginT1 = ( beginT + divT * 15.f ) * ( 1.f / 16.f ); // 1/16だけ始点と終点を混ぜる。0.001などを足すと範囲が狭くなった時に両端を越えてしまうからだ。
			}else{ //
				endT0 = endT;
				beginT1 = endT;
			}
			//找到起点。一个元素就足够了
			float begin = a[ mAxis ] + beginT * b[ mAxis ];
			//计算误差。
			float e = GameLib::Math::abs( a[ mAxis ] ) + GameLib::Math::abs( beginT * b[ mAxis ] ) + GameLib::Math::abs( mInner.mLine );
			e *= EPSILON * 10.f; //但是以防万一10倍
			//起点：左
			if ( begin + e < mInner.mLine ){
				if ( l && l->isIntersect( q, beginT, endT0 ) ){
					return true;
				}
				if ( beginT1 < endT ){
					if ( r && r->isIntersect( q, beginT1, endT ) ){
						return true;
					}
				}
			}else if ( begin - e > mInner.mLine ){ //起点：右
				if ( r && r->isIntersect( q, beginT, endT0 ) ){
					return true;
				}
				if ( beginT1 < endT ){
					if ( l && l->isIntersect( q, beginT1, endT ) ){
						return true;
					}
				}
			}else{ //不知道
				//由于这种情况很少见，因此时间范围是整体。
				if ( l && l->isIntersect( q, beginT, endT ) ){
					return true;
				}
				if ( r && r->isIntersect( q, beginT, endT ) ){
					return true;
				}
			}
		}
		return false;
	}
	//递归判断函数。返回三角形编号
	void getIntersection(
	int* minIndex, //最小时刻碰撞的三角形编号
	float* minT, //最小时刻
	const Query& q,
	float beginT, //a和b的值保持不变，并且箱子的大小由a'=a+beginT*b,b'=a+endT*b算出。
	float endT ) const {
		//
		if ( mAxis == -1 ){ //如果分割轴为-1
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				int triIdx = mOuter.mIndices[ i ];
++G;
				float t;
				if ( getIntersectionTriangleAndLineSegment( &t, q, triIdx ) ){ //判断原始线段。
					if ( t < *minT ){
						*minT = t; //更新
						*minIndex = triIdx;
					}
				}
			}
		}else{ //
			//别名
			const Vector3& a = q.mBegin;
			const Vector3& b = q.mVector;
			const Node* l = mInner.mLeft;
			const Node* r = mInner.mRight;
			//计算和平面相交的时刻。从d = a + bt得到 t =（d-a）/ b
			float divT;
			if ( b[ mAxis ] == 0.f ){ //分母0
				divT = FLOAT_MAX;
			}else{
				divT = ( mInner.mLine - a[ mAxis ] ) / b[ mAxis ];
			}
			float endT0, beginT1; //第一个节点的结束时间，下一个节点的开始时间
			if ( divT >= endT ){ //不到达下一个节点
				endT0 = endT;
				beginT1 = endT;
			}else if ( divT > beginT ){ //赶到下一个节点
				//如果没有计算错误，请照原样传递divT，
				endT0 = ( endT + divT * 15.f ) * ( 1.f / 16.f );
				beginT1 = ( beginT + divT * 15.f ) * ( 1.f / 16.f ); //只有 1/16起点和终点混合
			}else{ //
				endT0 = endT;
				beginT1 = endT;
			}
			//找到起点。一个元素就足够了
			float begin = a[ mAxis ] + beginT * b[ mAxis ];
			//计算误差。
			float e = GameLib::Math::abs( a[ mAxis ] ) + GameLib::Math::abs( beginT * b[ mAxis ] ) + GameLib::Math::abs( mInner.mLine );
			e *= EPSILON * 10.f; //但是以防万一10倍
			//起点：左
			if ( begin + e < mInner.mLine ){
				if ( l ){
					l->getIntersection( minIndex, minT, q, beginT, endT0 );
				}
				if ( beginT1 < endT ){
					if ( r ){
						r->getIntersection( minIndex, minT, q, beginT1, endT );
					}
				}
			}else if ( begin - e > mInner.mLine ){ //起点：右
				if ( r ){
					r->getIntersection( minIndex, minT, q, beginT, endT0 );
				}
				if ( beginT1 < endT ){
					if ( l ){
						l->getIntersection( minIndex, minT, q, beginT1, endT );
					}
				}
			}else{ //不知道
				//由于这种情况很少见，因此时间范围是整体。
				if ( l ){
					l->getIntersection( minIndex, minT, q, beginT, endT );
				}
				if ( r ){
					r->getIntersection( minIndex, minT, q, beginT, endT );
				}
			}
		}
	}
	//递归构造函数

	void build(
	const Vector3& minV, //最小范围
	const Vector3& maxV, //最大范围
	BuildArgs* args,
	int restLevel ){ //拆分多少次
		//
		OuterNode* outer = &mOuter;

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
		bool* hitR = hitL + outer->mIndexNumber;
		//左右三角形的数量
		int cl = 0;
		int cr = 0;
		//判断其所属
		const Triangle* triangles = args->mTriangles;
		const Vector3* vertices = args->mVertices;
		for ( int i = 0; i < outer->mIndexNumber; ++i ){ //找出每个三角形
			const Triangle& tri = triangles[ outer->mIndices[ i ] ];
			const Vector3& p0 = vertices[ tri.mIndices[ 0 ] ];
			const Vector3& p1 = vertices[ tri.mIndices[ 1 ] ];
			const Vector3& p2 = vertices[ tri.mIndices[ 2 ] ];
			//获取轴的最大最小值
			float minP, maxP;
			minP = min( p0[ axis ], p1[ axis ] );
			minP = min( minP, p2[ axis ] );
			maxP = max( p0[ axis ], p1[ axis ] );
			maxP = max( maxP, p2[ axis ] );

			hitL[ i ] = ( minP < div ); //
			hitR[ i ] = ( maxP > div ); //显然属于右侧
			//如果分界线是完美的。
			//考虑误差。
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
		//在这里变为InnerNode。OuterNode成员临时保存到局部变量
		//不应该过多使用union，因为这可能会造成混淆，但是如果出于性能原因可以这样做。
		int* indices = outer->mIndices;
		int indexNumber = outer->mIndexNumber;
		outer->mIndices = 0;
		outer->mIndexNumber = 0;
		outer = 0; //此后不能使用Outer

		InnerNode* inner = &mInner; //后面仅访问inner

		//InnerNode是mAxis包含除-1之外的任何符号的标志。
		mAxis = axis; //现在，如果axis == 0则不变，如果是1则为Y，如果为2，则为Z
		inner->mLine = div; //写分界线

		//分配左右节点
		if ( cl > 0 ){ //如果是三角形
			inner->mLeft = args->mNodePos;
#ifdef STRONG_DEBUG
Vector3 boxDiv = maxV;
boxDiv[ axis ] = div;
inner->mLeft->mMin = minV;
inner->mLeft->mMax = boxDiv;
inner->mLeft->mOrigN = cl;
#endif
			++( args->mNodePos );
			inner->mLeft->mOuter.mIndices = args->mIndexPool.allocate( cl );
		}
		if ( cr > 0 ){
			inner->mRight = args->mNodePos;
#ifdef STRONG_DEBUG
Vector3 boxDiv = minV;
boxDiv[ axis ] = div;
inner->mRight->mMin = boxDiv;
inner->mRight->mMax = maxV;
inner->mRight->mOrigN = cr;
#endif
			++( args->mNodePos );
			inner->mRight->mOuter.mIndices = args->mIndexPool.allocate( cr );
		}
		//分布到左右节点。
		for ( int i = 0; i < indexNumber; ++i ){ //找出每个三角形
			if ( hitL[ i ] ){
				OuterNode* l = &inner->mLeft->mOuter;
				l->mIndices[ l->mIndexNumber ] = indices[ i ];
				++( l->mIndexNumber );
			}
			if ( hitR[ i ] ){
				OuterNode* r = &inner->mRight->mOuter;
				r->mIndices[ r->mIndexNumber ] = indices[ i ];
				++( r->mIndexNumber );
			}
		}
		//
		if ( restLevel > 1 ){ //
			if ( cl > 1 ){
				Vector3 boxDiv = maxV;
				boxDiv[ axis ] = div;
				inner->mLeft->build( minV, boxDiv, args, restLevel - 1 );
			}
			if ( cr > 1 ){
				Vector3 boxDiv = minV;
				boxDiv[ axis ] = div;
				inner->mRight->build( boxDiv, maxV, args, restLevel - 1 );
			}
		}
	}
	void count( int* nodeNumber, int* indexNumber ){
		++( *nodeNumber ); //
		if ( mAxis != -1 ){
			if ( mInner.mLeft ){
				mInner.mLeft->count( nodeNumber, indexNumber );
			}
			if ( mInner.mRight ){
				mInner.mRight->count( nodeNumber, indexNumber );
			}
		}else{
			*indexNumber += mOuter.mIndexNumber;
		}		
	}
	Node* copy( Node** nodePos, int** indexPos ){
		Node* to = *nodePos; //复制目标
		++( *nodePos );
#ifdef STRONG_DEBUG
to->mMin = mMin;
to->mMax = mMax;
to->mOrigN = mOrigN;
#endif
		to->mAxis = mAxis;
		if ( mAxis != -1 ){ //对于内部节点
			to->mInner.mLine = mInner.mLine;
			if ( mInner.mLeft ){
				to->mInner.mLeft = mInner.mLeft->copy( nodePos, indexPos );
			}
			if ( mInner.mRight ){
				to->mInner.mRight = mInner.mRight->copy( nodePos, indexPos );
			}
		}else{ //分配索引
			to->mOuter.mIndices = *indexPos;
			//复制索引
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				to->mOuter.mIndices[ i ] = mOuter.mIndices[ i ];
			}
			to->mOuter.mIndexNumber = mOuter.mIndexNumber;
			( *indexPos ) += mOuter.mIndexNumber;
		}
		return to;
	}
	//删除底部的无用节点。三种删除方式
	//1.左为外点，右为空。把左边的内容提到自己身上。
	//2.右边是外点，左边是空。把右边的内容提到自己身上。
	//3.左右为外点，三角形列表相同。把孩子删掉复印给自己。
	void optimize(){
		if ( mAxis == -1 ){ //不处理外部点。结束
			return;
		}
		Node* l = mInner.mLeft;
		Node* r = mInner.mRight;
		//递归调用
		if ( l ){
			l->optimize();
		}
		if ( r ){
			r->optimize();
		}
		bool lIsOuter = ( l && ( l->mAxis == -1 ) );
		bool rIsOuter = ( r && ( r->mAxis == -1 ) );
		OuterNode* o = 0; //如果有外部问题，请放在此处。
		if ( lIsOuter && !r ){ //左边是外点，右边是空的。
			o = &l->mOuter;
		}else if ( rIsOuter && !l ){ //右外点，左为空
			o = &r->mOuter;
		}else if ( lIsOuter && rIsOuter ){ //相同检测
			OuterNode* lo = &l->mOuter;
			OuterNode* ro = &r->mOuter;
			bool match = true;
			if ( lo->mIndexNumber == ro->mIndexNumber ){
				for ( int i = 0; i < lo->mIndexNumber; ++i ){
					if ( lo->mIndices[ i ] != ro->mIndices[ i ] ){
						match = false;
					}
				}
				if ( match ){ //全部匹配。向左拉。
					o = lo;
				}
			}
		}
		if ( o ){ //
			mAxis = -1;
			mInner.mLeft = mInner.mRight = 0;
			mOuter.mIndexNumber = o->mIndexNumber;
			mOuter.mIndices = o->mIndices;
		}
	}
	void checkTriangleIndexExistance( Array< bool >& f ){
		if ( mAxis == -1 ){
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				f[ mOuter.mIndices[ i ] ] = true;
			}
		}else{
			if ( mInner.mLeft ){
				mInner.mLeft->checkTriangleIndexExistance( f );
			}
			if ( mInner.mRight ){
				mInner.mRight->checkTriangleIndexExistance( f );
			}
		}
	}
	void checkTriangle( const Triangle* triangles, const Vector3* vertices ) const {
		if ( mAxis == -1 ){ //仅检查内部点
			return;
		}
		if ( mInner.mLeft ){
			if ( mInner.mLeft->mAxis == -1 ){
				const OuterNode& o = mInner.mLeft->mOuter;
				for ( int i = 0; i < o.mIndexNumber; ++i ){
					int triIdx = o.mIndices[ i ];
					const Triangle& tri = triangles[ triIdx ];
					bool hit = false;
					for ( int j = 0; j < 3; ++j ){
						const Vector3& p = vertices[ tri.mIndices[ j ] ];
						for ( int k = 0; k < 3; ++k ){
							if ( p[ k ] < mInner.mLine ){
								hit = true;
							}
						}
					}
					STRONG_ASSERT( hit );
				}
			}else{
				mInner.mLeft->checkTriangle( triangles, vertices );
			}
		}
		if ( mInner.mRight ){
			if ( mInner.mRight->mAxis == -1 ){
				const OuterNode& o = mInner.mRight->mOuter;
				for ( int i = 0; i < o.mIndexNumber; ++i ){
					int triIdx = o.mIndices[ i ];
					const Triangle& tri = triangles[ triIdx ];
					bool hit = false;
					for ( int j = 0; j < 3; ++j ){
						const Vector3& p = vertices[ tri.mIndices[ j ] ];
						for ( int k = 0; k < 3; ++k ){
							if ( p[ k ] >= mInner.mLine ){
								hit = true;
							}
						}
					}
					STRONG_ASSERT( hit );
				}
			}else{
				mInner.mRight->checkTriangle( triangles, vertices );
			}
		}
	}
#ifdef STRONG_DEBUG
Vector3 mMin; //用于调试
Vector3 mMax;
int mOrigN;
#endif
	int mAxis; //分割轴。0：X，1：Y，2：Z，-1是OuterNode。
	union{ //内存在union中共享。所需的成员在结尾和内部是不同的，因此这样做是为了提高性能。
		InnerNode mInner;
		OuterNode mOuter;
	};
};

} //namespace{}

namespace GameLib{
namespace Scene{

class CollisionMesh::Impl : public ReferenceType {
public:
	explicit Impl( const char* filename ) :
	mVertices( 0 ), 
	mVertexNumber( 0 ),
	mTriangles( 0 ),
	mTriangleNumber( 0 ),
	mNodes( 0 ),
	mIndicesInNode( 0 ){
		mDocument = Document::create( filename );
	}
	explicit Impl( ConstElement e ) : 
	mVertices( 0 ), 
	mVertexNumber( 0 ),
	mTriangles( 0 ),
	mTriangleNumber( 0 ),
	mNodes( 0 ),
	mIndicesInNode( 0 ){
		createFromElement( e );
	}
	~Impl(){
		SAFE_DELETE_ARRAY( mVertices );
		SAFE_DELETE_ARRAY( mTriangles );
		SAFE_DELETE_ARRAY( mNodes );
		SAFE_DELETE_ARRAY( mIndicesInNode );
	};
	void createFromElement( ConstElement e ){
		//第一路径。计算顶点和三角形的数量。
		int n = e.childNumber();
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				mVertexNumber += child.childNumber();
			}else if ( name == "IndexBuffer" ){
				mTriangleNumber += child.childNumber();
			}
		}
		//
		mVertices = NEW Vector3[ mVertexNumber ];
		mTriangles = NEW Triangle[ mTriangleNumber ];
		//顶点填充
		int vertexPos = 0;
		int trianglePos = 0;
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				vertexPos = fillVertices( child, vertexPos );
			}else if ( name == "IndexBuffer" ){
				trianglePos = fillTriangles( child, trianglePos );
			}
		}
		//k-d tree生成
		buildKDTree();
	}
	void buildKDTree(){
		//分割时准备尽可能多的节点
		//获得等于或大于三角形数tn的最小2次幂值，并取其二倍值
		int tn = mTriangleNumber;
		int maxNodeNumber = 1;
		int maxLevel = 0; //最大分割数
		while ( maxNodeNumber < tn ){
			maxNodeNumber <<= 1;
			++maxLevel;
		}
		Node* tmpNodes = NEW Node[ maxNodeNumber * 2 ];
		//准备参数结构体
		BuildArgs args( mTriangles, mVertices, tmpNodes, mTriangleNumber );

		Node root;
		//初始化三角形编号
		root.mOuter.mIndices = args.mIndexPool.allocate( mTriangleNumber );
		root.mOuter.mIndexNumber = mTriangleNumber;
		for ( int i = 0; i < mTriangleNumber; ++i ){
			root.mOuter.mIndices[ i ] = i;
		}
		//测量世界的大小。检查XYZ的最大值和最小值。
		Vector3 minV( FLOAT_MAX ); //最大初始化
		Vector3 maxV( -minV.x ); //最小初始化
		for ( int i = 0; i < mVertexNumber; ++i ){
			minV.min( mVertices[ i ] ); //min取XYZ中的最小值
			maxV.max( mVertices[ i ] ); //max以XYZ取最大值。
		}
		root.build( minV, maxV, &args, maxLevel );
#ifdef STRONG_DEBUG
root.mMin = minV;
root.mMax = maxV;
#endif
		root.optimize(); //消除不必要的节点以提高性能。
		//接下来，测量所需区域并将其复制到固定区域。
		int indexNumber = 0;
		int nodeNumber = 0;
		root.count( &nodeNumber, &indexNumber );
		mIndicesInNode = NEW int[ indexNumber ];
		mNodes = NEW Node[ nodeNumber ];
		//复制所有节点
		Node* nodePos = mNodes;
		int* indicesPos = mIndicesInNode;
		root.copy( &nodePos, &indicesPos );
		//清理工作
		SAFE_DELETE_ARRAY( tmpNodes );
#ifdef STRONG_DEBUG
		Array< bool > triangleExistFlags( mTriangleNumber );
		for ( int i = 0; i < mTriangleNumber; ++i ){
			triangleExistFlags[ i ] = false;
		}
		mNodes[ 0 ].checkTriangleIndexExistance( triangleExistFlags );
		for ( int i = 0; i < mTriangleNumber; ++i ){
			STRONG_ASSERT( triangleExistFlags[ i ] );
		}
		mNodes[ 0 ].checkTriangle( mTriangles, mVertices );
#endif
	}
	bool isReady(){
		bool ok = true;
		if ( mDocument ){
			if ( mDocument.isReady() ){
				ConstElement e = mDocument.root();
				//
				if ( ( e.childNumber() == 1 ) && ( RefString( "Container" ) != e.name() ) ){
					e = e.child( 0 );
				}
				createFromElement( e );
				e.release(); //先丢弃。
				mDocument.release();
			}else{
				ok = false;
			}
		}
		return ok;
	}
	bool isIntersect( const Vector3& a, const Vector3& b ) const {
		Query q( a, b, mTriangles, mVertices );
		bool ret = mNodes[ 0 ].isIntersect( q, 0.f, 1.f );
#ifdef STRONG_DEBUG //粗糙版本进行比较
		bool retCheck = false;
		for ( int i = 0; i < mTriangleNumber; ++i ){
			float dummyT;
			if ( getIntersectionTriangleAndLineSegment( &dummyT, q, i ) ){
				retCheck = true;
			}
		}
		STRONG_ASSERT( ret == retCheck );
#endif
		return ret;
	}
	bool getIntersection(
	float* tOut,
	Vector3* nOut,
	const Vector3& a,
	const Vector3& b ) const {
G = 0;
		float t = FLOAT_MAX; //从这里出发
		int index = -1;
		Query q( a, b, mTriangles, mVertices );
		mNodes[ 0 ].getIntersection( &index, &t, q, 0.f, 1.f );
#ifdef STRONG_DEBUG //粗糙版本进行比较
		int indexCheck = -1;
		float tCheck = FLOAT_MAX;
		for ( int i = 0; i < mTriangleNumber; ++i ){
			float tmpT;
			if ( getIntersectionTriangleAndLineSegment( &tmpT, q, i ) ){
				if ( tmpT < tCheck ){
					tCheck = tmpT;
					indexCheck = i;

				}
			}
		}
		STRONG_ASSERT( t == tCheck ); //索引可能不匹配。
#endif
		if ( index != -1 ){
			//存储最终碰撞的三角形的信息
			if ( tOut ){
				*tOut = t;
			}
			if ( nOut ){
				int i0 = mTriangles[ index ].mIndices[ 0 ];
				int i1 = mTriangles[ index ].mIndices[ 1 ];
				int i2 = mTriangles[ index ].mIndices[ 2 ];
				const Vector3& p0 = mVertices[ i0 ];
				const Vector3& p1 = mVertices[ i1 ];
				const Vector3& p2 = mVertices[ i2 ];
				Vector3 e0, e1; //Edge边缘
				e0.setSub( p1, p0 );
				e1.setSub( p2, p0 );
				nOut->setCross( e0, e1 );
				nOut->normalize();
			}
			return true;
		}else{
			return false;
		}
	}
private:
	int fillVertices( ConstElement vb, int pos ){
		int n = vb.childNumber();
		RefString positionStr( "position" );
		for ( int i = 0; i < n; ++i ){
			ConstElement v = vb.child( i );
			int an = v.attributeNumber();
			for ( int j = 0; j < an; ++j ){
				ConstAttribute a = v.attribute( j );
				if ( positionStr == a.name() ){
					a.getFloatValues( &mVertices[ pos ].x, 3 );
					++pos;
				}
			}
		}
		return pos;
	}
	int fillTriangles( ConstElement ib, int pos ){
		int n = ib.childNumber();
		for ( int i = 0; i < n; ++i ){
			ConstElement t = ib.child( i );
			ASSERT( t.attributeNumber() > 0 && "IndexBuffer : It's not valid element." );
			ConstAttribute a = t.attribute( 0 );
			a.getIntValues( mTriangles[ pos ].mIndices, 3 );
			++pos;
		}
		return pos;
	}
	Document mDocument;
	Vector3* mVertices;
	int mVertexNumber;
	Triangle* mTriangles;
	int mTriangleNumber;
	Node* mNodes; //0节目开头
	int* mIndicesInNode; //节点中的索引父数组
};

CollisionMesh::CollisionMesh( Impl* impl ) : mImpl( impl ){
	if ( mImpl ){
		mImpl->refer();
	}
}

CollisionMesh CollisionMesh::create( const char* filename ){
	CollisionMesh r;
	r.mImpl = NEW Impl( filename );
	return r;
}

CollisionMesh CollisionMesh::create( ConstElement e ){
	CollisionMesh r;
	r.mImpl = NEW Impl( e );
	return r;
}

bool CollisionMesh::isIntersect( const Vector3& a, const Vector3& b ) const {
	return mImpl->isIntersect( a, b );
}

bool CollisionMesh::getIntersection(
float* t,
Vector3* n,
const Vector3& a,
const Vector3& b ) const {
	return mImpl->getIntersection( t, n, a, b );
}

bool CollisionMesh::isReady(){
	return mImpl->isReady();
}

#define TYPE CollisionMesh
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

} //namespace Scene
}//namespace GameLib