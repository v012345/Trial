#ifndef INCLUDED_GAMELIB_SCENE_TREEIMPL_H
#define INCLUDED_GAMELIB_SCENE_TREEIMPL_H

#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Scene/Batch.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Scene/Animation.h"

#include "Scene/BatchImpl.h"
#include "Scene/Node.h"
#include "Scene/TreeTemplate.h"
#include "Scene/ContainerImpl.h"
#include "Scene/AnimationImpl.h"

namespace GameLib{
using namespace GameLib::Graphics;
namespace Scene{

class Tree::Impl : public ReferenceType{
public:
	Impl( TreeTemplate* tmpl ) : 
	mTemplate( tmpl ),
	mPosition( 0.f, 0.f, 0.f ),
	mAngle( 0.f, 0.f, 0.f ),
	mScale( 1.f, 1.f, 1.f ),
	mColor( 1.f, 1.f, 1.f ),
	mTransparency( 1.f ),
	mTime( 0.f ),
	mSpeed( 1.f ),
	mNodes( 0 ),
	mNodeNumber( 0 ){
		mTemplate->container()->refer(); //增加所有容器的参考计数

		//开始建造
		mNodeNumber = tmpl->nodeNumber();
		mNodes = static_cast< Node* >( OPERATOR_NEW( sizeof( Node ) * mNodeNumber ) );
		for ( int i = 0; i < mNodeNumber; ++i ){
			const NodeTemplate* src = tmpl->node( i ); 
			Node* dst = &mNodes[ i ];
			//构造
			new ( dst ) Node( src );

			//计算孩子的数量
			int child = src->mChild;
			int childNumber = 0;
			while ( child >= 0 ){
				++childNumber;
				child = tmpl->node( child )->mBrother;
			}
			Node** children = mChildrenPool.allocate( childNumber );
			//
			child = src->mChild;
			int j = 0;
			while ( child >= 0 ){
				children[ j ] = mNodes + child;
				child = tmpl->node( child )->mBrother;
				++j;
			}
			//给节点
			dst->setChildren( children, childNumber );
		}
	}
	~Impl(){
		for ( int i = 0; i < mNodeNumber; ++i ){
			mNodes[ i ].~Node();
		}
		OPERATOR_DELETE( mNodes );

		//检查父容器释放
		Container::Impl* c = mTemplate->container(); 
		c->release();
		if ( c->referenceCount() == 0 ){ //
			SAFE_DELETE( c );
		}
		mTemplate = 0;
	}
	void draw(){
		//创建一个世界矩阵
		Matrix34 w;
		w.setTranslation( mPosition );
		w.rotateY( mAngle.y );
		w.rotateX( mAngle.x );
		w.rotateZ( mAngle.z );
		w.scale( mScale );
		//绘制
		mNodes[ 0 ].draw( w, mColor, mTransparency );
	}
	void setAnimation( Animation::Impl* a ){
		if ( a ){ //如果有什么
			for ( int i = 0; i < mNodeNumber; ++i ){
				mNodes[ i ].setAnimation( a->node( mNodes[ i ].name() ) );
			}
		}else{ //没有东西
			for ( int i = 0; i < mNodeNumber; ++i ){
				mNodes[ i ].removeAnimation(); //初始化状态
			}
		}
		mTime = 0.f;
	}
	void setAnimationSpeed( float s ){
		mSpeed = s;
	}
	void updateAnimation( float t ){
		if ( t >= 0.f ){
			mTime = t;
		}
		for ( int i = 0; i < mNodeNumber; ++i ){
			mNodes[ i ].updateAnimation( mTime );
		}
		mTime += mSpeed;
	}
	TreeTemplate* mTemplate;
	Vector3 mPosition;
	Vector3 mAngle;
	Vector3 mScale;
	Vector3 mColor;
	float mTransparency;
	float mTime; //动画时间
	float mSpeed; //动画速度

	Node* mNodes; //0论资排辈
	int mNodeNumber;

	Pool< Node* > mChildrenPool; //在这里集中每个节点的子数组
};

} //namespace Scene
} //namespace GameLib

#endif