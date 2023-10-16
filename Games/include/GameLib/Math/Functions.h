#ifndef INCLUDED_GAMELIB_MATH_FUNCTIONS_H
#define INCLUDED_GAMELIB_MATH_FUNCTIONS_H

//数学库。角度单位是度。
namespace GameLib{
namespace Math{

//三角函数
float sin( float );
float cos( float );
float tan( float );
//反三角函数
float asin( float );
float acos( float );
float atan( float );
float atan2( float y, float x );

float pow( float a, float b ); //
float log2( float ); //2底对数
float log10( float ); //10底对数
float abs( float );//绝对值
float min( float, float ); //最小
float max( float, float ); //最大
float sqrt( float ); //平方根

const float EPSILON = 0.00000011920928955078125f; //浮点精度（1/2 ^ 23）
const float FLOAT_MAX = 1.7014117331926442990585209174226e+38; //浮点数的最大值（（2 ^ 24-1）/ 2 ^ 23 * 2 ^ 126）

bool isSpecial( float ); //如果NaN，Inf为真

} //namespace Math
} //namespace GameLib

#endif