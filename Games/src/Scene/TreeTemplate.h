#ifndef INCLUDED_GAMELIB_SCENE_TREETEMPLATE_H
#define INCLUDED_GAMELIB_SCENE_TREETEMPLATE_H

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"

#include "Scene/BatchImpl.h"
#include "Scene/NodeTemplate.h"
#include "Scene/Node.h"

namespace GameLib{

namespace Math{
	class Vector3;
}
namespace PseudoXml{
	class Element;
}

namespace Scene{
using namespace Math;

//树的原型。
class TreeTemplate{
public:
	TreeTemplate( PseudoXml::ConstElement&, Container::Impl& ); //在cpp中。
	~TreeTemplate(){
		SAFE_DELETE_ARRAY( mNodes );
		mName = 0; //由于它是容器中的字符串，因此在此处取消引用
		mContainer = 0;
	}
	int nodeNumber() const { 
		return mNodeNumber; 
	}
	const NodeTemplate* node( int i ) const { 
		return &mNodes[ i ]; 
	}
	const char* name() const { 
		return mName; 
	}
	Container::Impl* container(){
		return mContainer;
	}
private:
	int countNode( GameLib::PseudoXml::ConstElement&, int n );
	int build( PseudoXml::ConstElement&, Container::Impl&, int nodePos );

	NodeTemplate* mNodes; //0因为号码是固定的，所以不需要指向根的指针
	int mNodeNumber;
	const char* mName;
	Container::Impl* mContainer;
};

} //namespace Scene
} //namespace GameLib

#endif
