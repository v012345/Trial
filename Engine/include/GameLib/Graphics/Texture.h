#ifndef INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H
#define INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H

namespace GameLib{
namespace PseudoXml{
	class ConstElement;
}
namespace Graphics{

class Texture{
public:
	///根据元素生成。如果有path的话，添加在文件名之前
	static Texture create( PseudoXml::ConstElement, const char* path = 0 );
	//空的纹理
	static Texture create( int width, int height, bool createMipChain );
	//TGA或DDS。用扩展名识别。
	static Texture create( const char* filename );
	//把TGA或DDS的内容交给对方。如果最初写着DDS，则认为是DDS，除此之外的则认为是TGA。
	static Texture create( const char* fileData, int fileSize );
	///拿到名字（如果从伪xml开始载入就是该名字。其他为空。）
	const char* name() const;

	///载入结束了吗？（没有const是因为内部有处理）
	bool isReady(); 
	///是否发生读取错误？
	bool isError() const;

	///写入函数
	void lock( unsigned** address, int* pitch, int mipLevel = 0 );
	///部分写入函数
	void lock( 
		unsigned** address, 
		int* pitch, 
		int x,
		int y,
		int width,
		int height,
		int mipLevel = 0 );
	void unlock( unsigned** address, int mipLevel = 0 );
	//获取信息
	int width() const;
	int height() const;
	///转换为2的n次幂之前的原始宽度
	int originalWidth() const;
	///转换为2的n次幂之前的原始高度
	int originalHeight() const;
	int mipmapNumber() const;
	///写入文件。不检查成功失败。用于debug。
	void write( const char* filename );

	//用户不会意识到的函数
	Texture();
	Texture( const Texture& );
	~Texture();
	operator void*() const;
	Texture& operator=( const Texture& );
	void release();
	bool operator==( const Texture& ) const;
	bool operator!=( const Texture& ) const;
	bool operator<( const Texture& ) const;

	class Impl;
	Texture( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

} //namespace Graphics
} //namespace GameLib

#endif
