#ifndef INCLUDED_GAMELIB_HASH_H
#define INCLUDED_GAMELIB_HASH_H

namespace GameLib {

// 通用哈希计算类
template <class T> class Hash {
  public:
    unsigned value(const T&, unsigned mod) const;
    bool isEqual(const T&, const T&) const;
};

} // namespace GameLib
#include "GameLib/Base/Impl/HashImpl.h" //里面是这个

#endif
