#ifndef INCLUDED_GAMELIB_COMPRESSOR_H
#define INCLUDED_GAMELIB_COMPRESSOR_H

#include "GameLib/Base/Array.h"

namespace GameLib {

/// 解压缩类

class Compressor {
  public:
    /// 压缩
    static void compress(
        Array<char>* compressed,
        int* inPlaceDecompressionBufferSize, // 展开时所需的缓冲区大小
        const char* data, int size);
    /// 展开。展开失败时请看outSize是否正确。outSize过大时，会破坏内存，引发崩溃。
    static void decompress(char* outData, int* outSize, const char* data, int size);
};

} // namespace GameLib

#endif
