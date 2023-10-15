#ifndef INCLUDED_GAMELIB_PRIMENUMBER_H
#define INCLUDED_GAMELIB_PRIMENUMBER_H

namespace GameLib {
namespace PrimeNumber {
bool isPrimeNumber(int);
// 返回大于其参数的最小质数
int next(int);
// 返回小于其参数的最大质数
int previous(int);
// 整数的平方根。小数点后四舍五入
int sqrt(int);
} // namespace PrimeNumber
} // namespace GameLib

#endif
