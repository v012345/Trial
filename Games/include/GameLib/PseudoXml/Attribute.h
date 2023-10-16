#ifndef INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H
#define INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H

namespace GameLib{
namespace PseudoXml{

class Attribute{
public:
	//NULL没有明确给出尺寸时。因为如果放入-1就可以随意测量，所以也可以只给一个
	void set( const char* name, int nameSize, const char* value, int valueSize );
	void set( const char* name, const char* value );
	void set( const char* name, int value ); //int値
	void set( const char* name, float value ); //float值
	void set( const char* name, double value ); //double值
	void set( const char* name, const int* values, int number ); //整数数组
	void set( const char* name, const float* values, int number ); //float数组
	void set( const char* name, const double* values, int number ); //double型数组
	const char* value() const;
	const char* name() const;
	///失败的话会返回0，如果原本就是0.f则没有区别。
	int getIntValue() const;
	///失败的话会返回0.f，如果原本就是0.f则没有区别。
	float getFloatValue() const;
	///失败的话会返回0.0，如果原本就是0.0则没有区别。
	double getDoubleValue() const;
	///返回值是实际读取的元素个数
	int getIntValues( int* out, int number ) const;
	///返回值是实际读取的元素数
	int getFloatValues( float* out, int number ) const;
	///返回值是实际读取的元素个数
	int getDoubleValues( double* out, int number ) const;

	//以下用户无需关注
	Attribute();
	~Attribute();
	Attribute( const Attribute& );
	operator void*() const;
	Attribute& operator=( const Attribute& );
	void release();
	bool operator==( const Attribute& ) const;
	bool operator!=( const Attribute& ) const;

	class Impl;
private:
	friend class Element;
	friend class Tag;

	explicit Attribute( Impl* );
	Impl* mImpl;
};

//const版本
class ConstAttribute{
public:
	const char* value() const;
	const char* name() const;
	///如果失败，则返回0，但与原始零没有区别。
	int getIntValue() const;
	///失败的话会返回0.f，如果原本就是0.f则没有区别。
	float getFloatValue() const;
	///如果失败，则返回0.0，但与最初包含的0.0不能区分。
	double getDoubleValue() const;
	///返回值是实际读取的元素数
	int getIntValues( int* out, int number ) const;
	///返回值是实际读取的元素个数
	int getFloatValues( float* out, int number ) const;
	///返回值是实际读取的元素数
	int getDoubleValues( double* out, int number ) const;

	//以下用户无需关注
	ConstAttribute();
	ConstAttribute( const Attribute& );
	~ConstAttribute();
	operator void*() const;
	ConstAttribute& operator=( const Attribute& );
	void release();
	bool operator==( const Attribute& ) const;
	bool operator!=( const Attribute& ) const;
private:
	Attribute mBase;
};


} //namespace PseudoXml
} //namespace GameLib

#endif
