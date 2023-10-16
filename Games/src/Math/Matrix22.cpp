#include "GameLib/GameLib.h"
#include "GameLib/Math/Matrix22.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Math/Vector2.h"

namespace GameLib{
namespace Math{

Matrix22::Matrix22(){
}

void Matrix22::setIdentity(){
	m00 = m11 = 1.f;
	m01 = m10 = 0.f;
}

void Matrix22::setRotation( float r ){
	float s = sin( r );
	float c = cos( r );
	m00 = m11 = c;
	m01 = -s;
	m10 = s;
}

void Matrix22::setScaling( float xy ){
	m00 = m11 = xy;
	m01 = m10 = 0.f;
}

void Matrix22::setScaling( float x, float y ){
	m00 = x;
	m11 = y;
	m01 = m10 = 0.f;
}

void Matrix22::setScaling( const Vector2& a ){
	m00 = a.x;
	m11 = a.y;
	m01 = m10 = 0.f;
}

void Matrix22::rotate( float r ){
	float s = sin( r );
	float c = cos( r );
	float t = m00;
	m00 = t * c + m01 * s;
	m01 = t * ( -s ) + m01 * c;
	t = m10;
	m10 = t * c + m11 * s;
	m11 = t * ( -s ) + m11 * c;
}

void Matrix22::scale( float xy ){
	m00 *= xy;
	m01 *= xy;
	m10 *= xy;
	m11 *= xy;
}

void Matrix22::scale( float x, float y ){
	m00 *= x;
	m01 *= y;
	m10 *= x;
	m11 *= y;
}

void Matrix22::scale( const Vector2& a ){
	m00 *= a.x;
	m01 *= a.y;
	m10 *= a.x;
	m11 *= a.y;
}

void Matrix22::mul( Vector2* out, const Vector2& in ) const {
	float tx = in.x;
	out->x = m00 * tx + m01 * in.y;
	out->y = m10 * tx + m11 * in.y;
}

void Matrix22::operator*=( const Matrix22& a ){
	float tx = m00;
	m00 = tx * a.m00 + m01 * a.m10;
	m01 = tx * a.m01 + m01 * a.m11;
	tx = a.m10;
	m10 = tx * a.m00 + m11 * a.m10;
	m11 = tx * a.m01 + m11 * a.m11;
}

void Matrix22::setMul( const Matrix22& a, const Matrix22& b ){
	//a == *this，b == *this都必须考虑
	float t00, t01;
	t00 = a.m00 * b.m00 + a.m01 * b.m10;
	t01 = a.m00 * b.m01 + a.m01 * b.m11;

	float t10;
	t10 = a.m10 * b.m00 + a.m11 * b.m10;
	m11 = a.m10 * b.m01 + a.m11 * b.m11;

	m00 = t00; m01 = t01;
	m10 = t10;
}

float& Matrix22::operator()( int r, int c ){
	ASSERT( r >= 0 && r < 2 );
	ASSERT( c >= 0 && c < 2 );
	return ( &m00 )[ 2 * r + c ];
}

float Matrix22::operator()( int r, int c ) const {
	ASSERT( r >= 0 && r < 2 );
	ASSERT( c >= 0 && c < 2 );
	return ( &m00 )[ 2 * r + c ];
}

void Matrix22::transpose(){
	float t = m01;
	m01 = m10;
	m10 = t;
}

void Matrix22::setTransposed( const Matrix22& a ){
	m00 = a.m00;
	m01 = a.m10;
	m10 = a.m01;
	m11 = a.m11;
}

//Kramer方法中的逆矩阵

void Matrix22::setInverse( const Matrix22& a ){
	float delta = a.m00 * a.m11 - a.m01 * a.m10;
	float rcpDelta = 1.f / delta;
	float t = a.m00; //备份以防万一a==*this这种情况
	m00 = a.m11 * rcpDelta;
	m01 = -a.m01 * rcpDelta;
	m10 = -a.m10 * rcpDelta;
	m11 = t * rcpDelta;
}

void Matrix22::invert(){ 
	setInverse( *this ); //
}

bool Matrix22::operator==( const Matrix22& a ) const {
	for ( int i = 0; i < 2; ++i ){
		for ( int j = 0; j < 2; ++j ){
			if ( ( *this )( i, j ) != a( i, j ) ){
				return false;
			}
		}
	}
	return true;
}

bool Matrix22::operator!=( const Matrix22& a ) const {
	return !( operator==( a ) );
}

} //namespace Math
} //namespace GameLib
