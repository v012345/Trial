#ifndef INCLUDED_GAMELIB_SCENE_CONTAINERIMPL_H
#define INCLUDED_GAMELIB_SCENE_CONTAINERIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/HashMultiMap.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Scene/Batch.h"
#include "GameLib/Scene/Container.h"

#include "Scene/AnimationImpl.h"
#include "Scene/BatchImpl.h"
#include "Scene/TreeTemplate.h"

namespace GameLib{
namespace Scene{
using namespace PseudoXml;
using namespace Graphics;

class Container::Impl : public ReferenceType{
public:
	Impl( ConstElement e, const char* extraDataPath ){
		ASSERT( RefString( "Container" ) == e.name() );
		if ( extraDataPath ){
			RefString tmp( extraDataPath );
			mExtraDataPath = allocateString( tmp );
		}else{
			mExtraDataPath = allocateString( RefString( ".\\" ) ); //这里
		}
		createFromElement( e );
	}
	Impl( const char* filename, const char* extraDataPath ){
		if ( extraDataPath ){
			RefString tmp( extraDataPath );
			mExtraDataPath = allocateString( tmp );
		}else{
			string t = filename; //输入文件名一次
			// 将/转换为\。记录最后找到的位置
			int slashPos = -1;
			for ( size_t i = 0; i < t.size(); ++i ){
				if ( t[ i ] == '/' ){
					t[ i ] = '\\';
				}
				if ( t[ i ] == '\\' ){
					slashPos = static_cast< int >( i );
				}
			}
			if ( slashPos == -1 ){	//如果为-1，则在末尾加 t = ""; //这里
			}else{
				t.erase( slashPos + 1, string::npos );
			}
			mExtraDataPath = allocateString( RefString( t.c_str() ) );
		}
		mDocument = Document::create( filename );
	}
	~Impl(){
		//VertexBuffer，IndexBuffer，Texture由引用计数器来管理
		//首先，各种Tree
		for ( int i = 0; i < mTrees.size(); ++i ){
			SAFE_DELETE( mTrees[ i ] );
		}
		//下一步销毁批次。
		for ( int i = 0; i < mBatches.size(); ++i ){
			SAFE_DELETE( mBatches[ i ] );
		}
		//最后动画
		for ( int i = 0; i < mAnimations.size(); ++i ){
			SAFE_DELETE( mAnimations[ i ] );
		}
		mExtraDataPath = 0; //因为它在stringPool中只需删除引用0
	}
	Batch::Impl* batch( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_BATCH ){
				return mBatches[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return 0;
	}
	Batch::Impl* batch( int id ) const {
		return mBatches[ id ];
	}
	Animation::Impl* animation( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_ANIMATION ){
				return mAnimations[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return 0;
	}
	Animation::Impl* animation( int id ) const {
		return mAnimations[ id ];
	}
	int animationNumber() const {
		return mAnimations.size();
	}
	int batchNumber() const {
		return mBatches.size();
	}
	TreeTemplate* tree( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_TREE ){
				return mTrees[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return 0;
	}
	TreeTemplate* tree( int id ) const {
		return mTrees[ id ];
	}
	int treeNumber() const {
		return mTrees.size();
	}
	VertexBuffer vertexBuffer( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_VERTEX_BUFFER ){
				return mVertexBuffers[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return VertexBuffer();
	}
	IndexBuffer indexBuffer( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_INDEX_BUFFER ){
				return mIndexBuffers[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return IndexBuffer();
	}
	Texture texture( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_TEXTURE ){
				return mTextures[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return Texture();
	}
	const char* allocateString( const RefString& s ){
		char* p = mStringPool.allocate( s.get(), s.size() + 1 );
		p[ s.size() ] = '\0';
		return p;
	}
	bool isReady(){
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
			}
		}
		if ( !mDocument ){ //如果加载完成并且所有纹理均已加载，则为True
			for ( int i = 0; i < mTextures.size(); ++i ){
				if ( !mTextures[ i ].isReady() ){
					return false;
				}
			}
			return true;
		}
		return false;
	}
private:
	//指示从哈希获得的下标是什么下标的标记。低16位是下标
	enum IndexPrefix{
		PREFIX_VERTEX_BUFFER = 0x10000,
		PREFIX_INDEX_BUFFER = 0x20000,
		PREFIX_TEXTURE = 0x30000,
		PREFIX_ANIMATION = 0x40000,
		PREFIX_BATCH = 0x50000,
		PREFIX_TREE = 0x60000,
	};
	void createFromElement( ConstElement e ){
		//
		int vertexBufferN = 0;
		int indexBufferN = 0;
		int textureN = 0;
		int animationN = 0;
		int batchN = 0;
		int treeN = 0;

		int n = e.childNumber();
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				++vertexBufferN;
			}else if ( name == "IndexBuffer" ){
				++indexBufferN;
			}else if ( name == "Texture" ){
				++textureN;
			}else if ( name == "Animation" ){
				++animationN;
			}else if ( name == "Batch" ){
				++batchN;
			}else if ( name == "Tree" ){
				++treeN;
			}
		}
		//
		mVertexBuffers.setSize( vertexBufferN );
		mIndexBuffers.setSize( indexBufferN );
		mTextures.setSize( textureN );
		mAnimations.setSize( animationN );
		mBatches.setSize( batchN );
		mTrees.setSize( treeN );

		mIndices.setCapacity( n );

		//开始创建。初始化一次该数字并将其用作位置
		vertexBufferN = 0;
		indexBufferN = 0;
		textureN = 0;
		animationN = 0;
		batchN = 0;
		treeN = 0;

		//创建不依赖的对象
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				mVertexBuffers[ vertexBufferN ] = VertexBuffer::create( child );
				mIndices.add( 
					mVertexBuffers[ vertexBufferN ].name(), 
					PREFIX_VERTEX_BUFFER | vertexBufferN );
				++vertexBufferN;
			}else if ( name == "IndexBuffer" ){
				mIndexBuffers[ indexBufferN ] = IndexBuffer::create( child );
				mIndices.add( 
					mIndexBuffers[ indexBufferN ].name(), 
					PREFIX_INDEX_BUFFER | indexBufferN );
				++indexBufferN;
			}else if ( name == "Texture" ){
				mTextures[ textureN ] = Texture::create( child, mExtraDataPath );
				mIndices.add( 
					mTextures[ textureN ].name(), 
					PREFIX_TEXTURE | textureN );
				++textureN;
			}else if ( name == "Animation" ){
				mAnimations[ animationN ] = new Animation::Impl( child, *this );
				mIndices.add( 
					mAnimations[ animationN ]->name(), 
					PREFIX_ANIMATION | animationN );
				++animationN;
			}
		}
		//根据这三种类型创建批处理
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "Batch" ){
				mBatches[ batchN ] = NEW Batch::Impl( child, *this );
				mIndices.add( 
					mBatches[ batchN ]->name(), 
					PREFIX_BATCH | batchN );
				++batchN;
			}
		}
		//创建一个依赖批处理的树
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "Tree" ){
				mTrees[ treeN ] = NEW TreeTemplate( child, *this );
				mIndices.add( 
					mTrees[ treeN ]->name(), 
					PREFIX_TREE | treeN );
				++treeN;
			}
		}
	}

	//可以使用Document.InFile加载文件，但这很麻烦。
	Document mDocument;

	//Graphics按引用类型存储
	Array< VertexBuffer > mVertexBuffers;
	Array< IndexBuffer > mIndexBuffers;
	Array< Texture > mTextures;
	Array< Animation::Impl* > mAnimations;
	//Scene本身就是Impl类型。
	Array< Batch::Impl* > mBatches;
	Array< TreeTemplate* > mTrees;

	//散用于查找名字的哈希。
	//第一种类型是引用，它取决于实体是否继续存在。注意
	//第二种类型是数组索引。
	//将HashMultiMap设置为int之上，以表明它是什么。
	//如果不这样做，那么如果纹理和树的名称相同，就会遇到麻烦。
	HashMultiMap< const char*, int > mIndices;

	//用于存储字符串的池。因为每种类型的new都很慢。
	Pool< char > mStringPool;

	//追加数据查找路径
	const char* mExtraDataPath;
};

} //namespace Scene
} //namespace GameLib

#endif

