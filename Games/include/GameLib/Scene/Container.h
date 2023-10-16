#ifndef INCLUDED_GAMELIB_SCENE_CONTAINER_H
#define INCLUDED_GAMELIB_SCENE_CONTAINER_H

namespace GameLib{

namespace PseudoXml{
	class ConstElement;
};

namespace Scene{

class Batch;
class Model;
class Tree;
class Animation;

///存放绘图数据的容器。

class Container{
public:
	///从伪xml生成。如果省略数据路径，默认指exe所在的位置
	static Container create( PseudoXml::ConstElement, const char* extraDataPath = 0 );
	///直接从文件生成。如果省略数据路径，默认指exe所在的位置
	static Container create( const char* filename, const char* extraDataPath = 0 );
	///根据批处理名称创建模型
	Model createModel( const char* batchName ) const;
	///根据批处理编号创建模型
	Model createModel( int batchId ) const;
	///指定树名来创建树
	Tree createTree( const char* treeName ) const;
	///指定树的编号来创建树
	Tree createTree( int treeId ) const;

	///检测载入结束。请确认会在调用后返回true再使用。
	bool isReady();

	Batch batch( const char* batchName ) const;
	Batch batch( int batchIndex ) const;
	Animation animation( const char* animationName ) const;
	Animation animation( int animationIndex ) const;
	//知道批次数量。如果要放入其中的所有内容，请将其与getBatch（int）一起使用。
	int batchNumber() const;
	//知道Tree的数量。如果要在其中获取所有内容，请与createTree（int）一起使用。
	int treeNumber() const;
	//可以看到动画数量。
	int animationNumber() const;

	//以下用户无需关注
	Container();
	~Container();
	Container( const Container& );
	operator void*() const;
	Container& operator=( const Container& );
	void release();
	bool operator==( const Container& ) const;
	bool operator!=( const Container& ) const;

	class Impl;
private:
	explicit Container( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
