#ifndef INCLUDED_GAMELIB_SCENE_CURVE_H
#define INCLUDED_GAMELIB_SCENE_CURVE_H

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Math/Functions.h"

namespace GameLib{
using namespace PseudoXml;
using namespace Math;
namespace Scene{

class Curve{
public:
	enum Type{
		TYPE_TX, //transX
		TYPE_TY, //transY
		TYPE_TZ, //transZ
		TYPE_RX, //rotateX
		TYPE_RY, //rotateY
		TYPE_RZ, //rotateZ
		TYPE_SX, //scaleX
		TYPE_SY, //scaleY
		TYPE_SZ, //scaleZ
		TYPE_SA, //scale ALL(XYZ)
		TYPE_UNKNOWN,
	};
	struct Key{
		Key() : mTime( 0.f ), mValue( 0.f ), mLeftSlope( 0.f ), mRightSlope( 0.f ){}
		float mTime;
		float mValue;
		float mLeftSlope;
		float mRightSlope;
	};
	Curve( PseudoXml::ConstElement& e ) :
	mKeys( 0 ),
	mKeyNumber( 0 ),
	mType( TYPE_UNKNOWN ), //适当的
	mInterporation( INTERPORATION_NONE ){
		ASSERT( RefString( "Curve" ) == e.name() ); //Curve？
		ASSERT( e.childNumber() > 0 ); //只有一个数据，对不对？
		//取得名字
		bool isRot = false; //由于文件记录的是度数，因此将其转换为弧度。
		int an = e.attributeNumber();
		for ( int i = 0; i < an; ++i ){
			ConstAttribute a = e.attribute( i );
			RefString name( a.name() );
			RefString value( a.value() );
			if ( name == "type" ){
				if ( value == "translationX" ){
					mType = TYPE_TX;
				}else if ( value == "translationY" ){
					mType = TYPE_TY;
				}else if ( value == "translationZ" ){
					mType = TYPE_TZ;
				}else if ( value == "rotationX" ){
					mType = TYPE_RX;
					isRot = true;
				}else if ( value == "rotationY" ){
					mType = TYPE_RY;
					isRot = true;
				}else if ( value == "rotationZ" ){
					mType = TYPE_RZ;
					isRot = true;
				}else if ( value == "scaleX" ){
					mType = TYPE_SX;
				}else if ( value == "scaleY" ){
					mType = TYPE_SY;
				}else if ( value == "scaleZ" ){
					mType = TYPE_SZ;
				}else if ( value == "scaleXYZ" ){
					mType = TYPE_SA;
				}
			}else if ( name == "interporation" ){
				if ( value == "none" ){
					mInterporation = INTERPORATION_NONE;
				}else if ( value == "linear" ){
					mInterporation = INTERPORATION_LINEAR;
				}else if ( value == "cubic" ){
					mInterporation = INTERPORATION_CUBIC;
				}
			}
		}
		mKeyNumber = e.childNumber();
		mKeys = static_cast< Key* >( OPERATOR_NEW( sizeof( Key ) * mKeyNumber ) );
		for ( int i = 0; i < mKeyNumber; ++i ){
			new ( &mKeys[ i ] ) Key;
			ConstElement c = e.child( i );
			an = c.attributeNumber();
			for ( int j = 0; j < an; ++j ){
				ConstAttribute a = c.attribute( j );
				RefString name( a.name() );
				float v = a.getFloatValue();
				if ( name == "time" ){
					mKeys[ i ].mTime = v;
				}else if ( name == "value" ){
					mKeys[ i ].mValue = v;
				}else if ( name == "leftSlope" ){
					mKeys[ i ].mLeftSlope = v;
				}else if ( name == "rightSlope" ){
					mKeys[ i ].mRightSlope = v;
				}else if ( name == "slope" ){
					mKeys[ i ].mRightSlope = mKeys[ i ].mLeftSlope = v;
				}else{
					ASSERT( false ); //
				}
			}
		}
	}
	~Curve(){
		mName = 0; //只需删除引用即可
		for ( int i = 0; i < mKeyNumber; ++i ){
			mKeys[ i ].~Key();
		}
		OPERATOR_DELETE( mKeys );
	}
	float operator()( float t ) const {
		//执行循环处理。最后的时间被视为一个周期
		//要计算可以从a中减去b的次数，请使用a -= toInt(a/b) * b计算。toInt为向下舍入为整数的函数。可以通过强制转换来实现。
		float quot = t / mKeys[ mKeyNumber - 1 ].mTime; //
		int quotInt = static_cast< int >( quot ); 
		t -= static_cast< float >( quotInt ) * mKeys[ mKeyNumber - 1 ].mTime;

		if ( mInterporation == INTERPORATION_NONE ){
			int last = 0;
			for ( int i = 0; i < mKeyNumber; ++i ){
				if ( mKeys[ i ].mTime > t ){
					break;
				}
				last = i;
			}
			//使用最短时间不超过t的数据（无插值）
			return mKeys[ last ].mValue;
		}else if ( mInterporation == INTERPORATION_LINEAR ){
			int begin = 0;
			int end = 0;
			for ( end = 0; end < mKeyNumber; ++end ){
				if ( mKeys[ end ].mTime > t ){
					break;
				}
				begin = end;
			}
			if ( end >= mKeyNumber ){ //防止溢出。返回最后一个值
				return mKeys[ mKeyNumber - 1 ].mValue;
			}
			float t0 = mKeys[ begin ].mTime;
			float t1 = mKeys[ end ].mTime;
			float p0 = mKeys[ begin ].mValue;
			float p1 = mKeys[ end ].mValue;
			//变量转换
			t = ( t - t0 ) / ( t1 - t0 );
			//线性补间
			return p0 + ( p1 - p0 ) * t;
		}else if ( mInterporation == INTERPORATION_CUBIC ){
			int begin = 0;
			int end = 0;
			for ( end = 0; end < mKeyNumber; ++end ){
				if ( mKeys[ end ].mTime > t ){
					break;
				}
				begin = end;
			}
			if ( end >= mKeyNumber ){ //防止溢出。返回最后一个值
				return mKeys[ mKeyNumber - 1 ].mValue;
			}
			float t0 = mKeys[ begin ].mTime;
			float t1 = mKeys[ end ].mTime;
			float p0 = mKeys[ begin ].mValue;
			float p1 = mKeys[ end ].mValue;
			float v0 = mKeys[ begin ].mRightSlope; //起点的右斜率
			float v1 = mKeys[ end ].mLeftSlope; //终点向左倾斜
			//变量转换
			t = ( t - t0 ) / ( t1 - t0 );
			//以at^3 + bt^2 + c + d计算。c = v0，d = p0。
			float a = 2.f * ( p0 - p1 ) + ( v0 + v1 );
			float b = 3.f * ( p1 - p0 ) - ( 2.f * v0 ) - v1;
			//线性补间
			float r = a; //a
			r *= t; //at
			r += b; //at+b
			r *= t; //at^2+bt
			r += v0; //at^2+bt+c
			r *= t; //at^3+bt^2+ct
			r += p0; //at^3+bt^2+ct+d
			return r;
		}else{
			ASSERT( false ); //
			return 0.0;
		}
	}
	Type type() const {
		return mType;
	}
private:
	enum Interporation{
		INTERPORATION_NONE, //无插值
		INTERPORATION_LINEAR, //线性的
		INTERPORATION_CUBIC, //3下一个
	};
	Key* mKeys;
	int mKeyNumber;
	const char* mName;
	Type mType;
	Interporation mInterporation;
};

} //namespace Scene
} //namespace GameLib

#endif

