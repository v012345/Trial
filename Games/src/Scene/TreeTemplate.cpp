#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Scene/Container.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Math/Functions.h"

#include "Scene/TreeTemplate.h"
#include "Scene/ContainerImpl.h"
#include "Scene/NodeTemplate.h"

using namespace GameLib;
using namespace GameLib::PseudoXml;
using namespace GameLib::Scene;

namespace GameLib{
namespace Scene{

TreeTemplate::TreeTemplate( ConstElement& e, Container::Impl& container ) :
mNodes( 0 ),
mNodeNumber( 0 ),
mName( 0 ),
mContainer( &container ){
	ASSERT( e.childNumber() == 1 && "Tree must have 1 root node. this data is invalid. " ); //只有一个根节点，对吗
	//取得名字
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		ConstAttribute a = e.attribute( i );
		if ( RefString( "name" ) == a.name() ){
			RefString value( a.value() );
			mName = container.allocateString( value );
		}
	}
	//首先计算节点
	ConstElement root = e.child( 0 ); //
	mNodeNumber = countNode( root, 0 );
	mNodes = NEW NodeTemplate[ mNodeNumber ];
	//构建层级
	build( root, container, 0 );
}

//递归
int TreeTemplate::countNode( ConstElement& e, int n ){
	++n; //
	int cn = e.childNumber();
	for ( int i = 0; i < cn; ++i ){
		ConstElement c = e.child( i );
		n = countNode( c, n );
	}
	return n;
}

int TreeTemplate::build( ConstElement& e, Container::Impl& container, int nodePos ){
	//设置自己的信息
	bool hasTransform = false; //如果有队列，请优先考虑该队列
	Vector3 translation( 0.f, 0.f, 0.f );
	Vector3 rotation( 0.f, 0.f, 0.f );
	Vector3 scale( 1.f, 1.f, 1.f );

	int an = e.attributeNumber();
	NodeTemplate& node = mNodes[ nodePos ];
	for ( int i = 0; i < an; ++i ){
		ConstAttribute a = e.attribute( i );
		RefString name( a.name() );

		if ( name == "name" ){
			RefString value( a.value() );
			node.mName = container.allocateString( value );
		}else if ( name == "batch" ){
			node.mBatch = container.batch( a.value() );
		}else if ( name == "translation" ){
			a.getFloatValues( &translation.x, 3 );
		}else if ( name == "rotation" ){
			a.getFloatValues( &rotation.x, 3 );
		}else if ( name == "scale" ){
			a.getFloatValues( &scale.x, 3 );
		}else if ( name == "transform" ){
			a.getFloatValues( &node.mTransform.m00, 12 );
			hasTransform = true;
		}
	}
	if ( !hasTransform ){
		node.mTransform.setTranslation( translation );
		node.mTransform.rotateY( rotation.y );
		node.mTransform.rotateX( rotation.x );
		node.mTransform.rotateZ( rotation.z );
		node.mTransform.scale( scale );
	}
	++nodePos;
	ASSERT( nodePos <= mNodeNumber );  //不可能的
	//插入孩子的信息
	int cn = e.childNumber();
	int elder = -1;
	for ( int i = 0; i < cn; ++i ){
		ConstElement c = e.child( i );
		if ( i == 0 ){ //0如果是第一个孩子的话就给自己设定
			node.mChild = nodePos;
		}else{ //否则，到前一个子结点
			mNodes[ elder ].mBrother = nodePos;
		}
		elder = nodePos;
		nodePos = build( c, container, nodePos );
	}
	return nodePos;
}

} //namespace Scene
} //namespace GameLib

