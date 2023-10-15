#include "GameLib/Base/Compressor.h"
#include "GameLib/GameLib.h"

#define OPTIMIZATION // 如果启用，则为高性能版本。

namespace GameLib {

namespace {

// 位分配常数
const int DIC_BITS = 11; // 位置bit数
// 自动计算常数
const int LENGTH_BITS = 16 - 1 - DIC_BITS; // 长度
const int DIC_MASK = (1 << DIC_BITS) - 1;
const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; // 低8位
const int DIC_MASK_SHIFTED = (DIC_MASK >> 8) << LENGTH_BITS;
const int LENGTH_MASK = (1 << LENGTH_BITS) - 1;
const int DIC_SIZE = DIC_MASK + 1; // 字典大小（由于减去和存储，因此再使用一个）
const int MAX_LENGTH = LENGTH_MASK + 3; // 最大匹配长度（由于减去并存储了3个，因此可以再使用3个）

// 常用的最小值和最大值
inline int min(int a, int b) { return (a < b) ? a : b; }

inline int max(int a, int b) { return (a > b) ? a : b; }

#ifdef OPTIMIZATION

class Node {
  public:
    int mNext;
    int mPrev;
    int mPos; // 在文件中的位置
};

class Index { // 最多使用DIC_SIZE个节点。接下来的256个是虚拟磁头。DIC_SIZE + c表示字母c列表的开头。
  public:
    Index() {
        // dummy头的初始化。next，prev初始化为指向自己
        for (int i = DIC_SIZE; i < (DIC_SIZE + 256); ++i) { mNodes[i].mNext = mNodes[i].mPrev = i; }
        // 初始化记录空位置的堆栈。最多DIC_SIZE。
        for (int i = 0; i < DIC_SIZE; ++i) { mStack[i] = i; }
        mStackPos = DIC_SIZE;
    }
    int getFirst(unsigned char c) const { return mNodes[DIC_SIZE + c].mNext; }
    const Node* getNode(int i) const { return &mNodes[i]; }
    // 添加到顶部。较新的将被更快地搜索。
    void add(unsigned char c, int pos) {
        --mStackPos;
        int newIdx = mStack[mStackPos];
        Node* newNode = mNodes + newIdx;
        Node* head = &mNodes[DIC_SIZE + c];
        newNode->mNext = head->mNext;
        newNode->mPrev = DIC_SIZE + c;
        newNode->mPos = pos;
        mNodes[head->mNext].mPrev = newIdx;
        head->mNext = newIdx;
    }
    // 从末尾开始搜索并删除它。要删除的东西是第一个添加的东西，因此绝对是最后一个。
    // 无需循环列表。
    void remove(unsigned char c, int pos) {
        int idx = mNodes[DIC_SIZE + c].mPrev;
        Node* n = &mNodes[idx];
        ASSERT(n->mPos == pos); // 一定在这里，如果不满足此条件，那就是一个错误。
        mStack[mStackPos] = mNodes[n->mPrev].mNext; // 删掉
        ++mStackPos;
        mNodes[n->mPrev].mNext = n->mNext;
        mNodes[n->mNext].mPrev = n->mPrev;
    }
    bool isEnd(int idx) const { // 返回是否为dummy头。如果大于DIC_SIZE则为虚拟。
        return (idx >= DIC_SIZE);
    }

  private:
    Node mNodes[DIC_SIZE + 256];
    int mStack[DIC_SIZE];
    int mStackPos;
};

#endif // OPTIMIZATION

} // namespace

void Compressor::compress(Array<char>* oDataSigned, int* inPlaceDecompressionBufferSize, const char* iDataSigned, int iSize) {
    int worstSize = iSize + (iSize / 128) + 1; // 按最坏的情况计算。
    oDataSigned->setSize(worstSize);

    unsigned char* oData = reinterpret_cast<unsigned char*>(&(*oDataSigned)[0]);
    const unsigned char* iData = reinterpret_cast<const unsigned char*>(iDataSigned);

    int oPos = 0; // 写入侧的写入位置
    int i = 0;
    int unmatchBegin = 0; // 不匹配区域的起始位置
#ifdef OPTIMIZATION
    Index index;
#endif
    while (i < iSize) {
        // 从字典中搜索
        int matchLength = 0;
        int matchPos = 0;
        // 从字典的开头搜索。j不超过i。
        // 最大搜寻长度
        int maxL = min(MAX_LENGTH, iSize - i); // 由于无法搜索超出文件末尾的位置，因此请限制maxL。
#ifdef OPTIMIZATION
        // 开始搜索查找第一个字符
        int idx = index.getFirst(iData[i]);
        while (!index.isEnd(idx)) {
            const Node* n = index.getNode(idx);
            int p = n->mPos;
            // 查找匹配长度
            int l = 1; // 1从文字一致状态开始。
            while (l < maxL) {
                // 如果下一个字符不匹配则结束
                if (iData[p + l] != iData[i + l]) { break; }
                ++l; // 1文字成长
            }
            // 记录是否比以前更长。匹配的方式很多，记录最大的一种。
            if (matchLength < l) {
                matchPos = p;
                matchLength = l;
                if (matchLength == maxL) { // 当达到最大匹配长度时，它就到此为止。
                    break;
                }
            }
            idx = n->mNext;
        }
        // 顺便说一句，如果超过3个字符，它将以压缩模式记录。
        if (matchLength >= 3) {
            // 字典更新。关闭并添加
            for (int j = 0; j < matchLength; ++j) {
                int delPos = i + j - DIC_SIZE;
                if (delPos >= 0) { index.remove(iData[delPos], delPos); }
                index.add(iData[i + j], i + j);
            }
            // 写入未压缩的标头
            if (unmatchBegin < i) {
                oData[oPos] = static_cast<unsigned char>(i - unmatchBegin - 1); // 至少1，减去1并保存
                ++oPos;
                for (int j = unmatchBegin; j < i; ++j) {
                    oData[oPos] = iData[j];
                    ++oPos;
                }
            }
            // 记录压缩部分
            int wl = matchLength - 3; // 3保存引用
            int wp = i - matchPos - 1; // 1保存引用
            int tmp = 0x80 | wl; // 将压缩标志添加到长度的位
            tmp |= (wp & DIC_MASK_HIGH) >> (8 - LENGTH_BITS); //
            oData[oPos + 0] = static_cast<unsigned char>(tmp);
            oData[oPos + 1] = static_cast<unsigned char>(wp & 0xff);
            oPos += 2;
            i += matchLength;
            unmatchBegin = i; // 下一个开始是非一致位置
        } else { // 不匹配。
            // 字典更新。关闭并添加。
            int delPos = i - DIC_SIZE;
            if (delPos >= 0) { index.remove(iData[delPos], delPos); }
            index.add(iData[i], i);
            ++i;
            if (i - unmatchBegin == 128) { // 累积到一定程度。写入
                // 写入未压缩的标头
                oData[oPos] = static_cast<unsigned char>(i - unmatchBegin - 1); // 至少1，减去1并保存
                ++oPos;
                for (int j = unmatchBegin; j < i; ++j) {
                    oData[oPos] = iData[j];
                    ++oPos;
                }
                unmatchBegin = i;
            }
        }
#else //
      // 字典的开头。
        int dicBegin = max(i - DIC_SIZE, 0); // 注意不要放在0的前面。所以使用max（）
        for (int j = dicBegin; j < i; ++j) { // 该循环的内容占据了大多数计算。有必要在这里提升性能，但我认为这非常困难。
            // 查找匹配长度
            int l = 0;
            while (
                l <
                maxL) { // j<iで、i+l<sizeIn。よって、j+l<sizeInで、範囲内に入る。ここでj+l>=iはありうる。つまり、辞書をはみ出して検索することはありうる。しかし、それでも正しく動くのだ。図を描いて調べよう。
                // 如果下一个字符不匹配则结束
                if (iData[j + l] != iData[i + l]) { break; }
                ++l; // 1文字成长
            }
            // 记录是否比以前更长。匹配的方式很多，记录最大的一种。
            if (matchLength < l) {
                matchPos = j;
                matchLength = l;
                if (matchLength == maxL) { // 当达到最大匹配长度时，它就到此为止。
                    break;
                }
            }
        }
        // 顺便说一句，如果超过3个字符，它将以压缩模式记录。
        if (matchLength >= 3) {
            if (unmatchBegin < i) {
                // 写入未压缩的标头
                oData[oPos] = static_cast<unsigned char>(i - unmatchBegin - 1); // 至少1，减去1并保存
                ++oPos;
                for (int j = unmatchBegin; j < i; ++j) {
                    oData[oPos] = iData[j];
                    ++oPos;
                }
            }
            // 记录压缩部分
            int wl = matchLength - 3; // 3保存引用
            int wp = i - matchPos - 1; // 1保存引用
            int tmp = 0x80 | wl; // 将压缩标志添加到长度的位
            tmp |= (wp & DIC_MASK_HIGH) >> (8 - LENGTH_BITS); //
            oData[oPos + 0] = static_cast<unsigned char>(tmp);
            oData[oPos + 1] = static_cast<unsigned char>(wp & 0xff);
            oPos += 2;
            i += matchLength;
            unmatchBegin = i; // 下一个开始是非一致位置
        } else { // 不匹配。
            ++i;
            if (i - unmatchBegin == 128) { // 累积到一定程度。写入
                // 写入未压缩的标头
                oData[oPos] = static_cast<unsigned char>(i - unmatchBegin - 1); // 至少1，减去1并保存
                ++oPos;
                for (int j = unmatchBegin; j < i; ++j) {
                    oData[oPos] = iData[j];
                    ++oPos;
                }
                unmatchBegin = i;
            }
        }
#endif // OPTIMIZATION
    }

    // 如果仍然存在不匹配位置，则最后写入
    if (unmatchBegin < i) {
        // 非圧縮ヘッダ書き込み
        oData[oPos] = static_cast<unsigned char>(i - unmatchBegin - 1); // 最低1なので1引いて保存
        ++oPos;
        for (int j = unmatchBegin; j < i; ++j) {
            oData[oPos] = iData[j];
            ++oPos;
        }
    }
    // 计算需要量
    if (inPlaceDecompressionBufferSize) {
        // 展开并进行检查。
        int wp = 0; // 写指针
        int diff = 0; // 写指针-读指针
        int i = 0;
        int bufferSize = max(iSize, oPos); // 增长时使用oPo​​s。
        while (i < oPos) {
            int length;
            if (oData[i] & 0x80) { // 压缩方式
                length = oData[i] & LENGTH_MASK;
                length += 3; // 3记录得很少。
                i += 2;
            } else { // 未压缩模式
                length = oData[i] + 1; // 1记录得很少。
                i += length + 1;
            }
            wp += length;
            diff = max(diff, wp - (i + bufferSize - oPos));
        }
        *inPlaceDecompressionBufferSize = bufferSize + diff;
    }
    oDataSigned->setSize(oPos); // 应该缩小
}

// 部署非常容易。
void Compressor::decompress(char* oDataSigned, int* oSize, const char* iDataSigned, int iSize) {
    unsigned char* oData = reinterpret_cast<unsigned char*>(oDataSigned);
    const unsigned char* iData = reinterpret_cast<const unsigned char*>(iDataSigned);

    int oPos = 0;
    for (int i = 0; i < iSize; ++i) {
        int length;
        if (iData[i] & 0x80) { // 压缩方式
            length = iData[i] & LENGTH_MASK;
            length += 3; // 3记录得很少。
            int position = ((iData[i] & DIC_MASK_SHIFTED) << (8 - LENGTH_BITS)) | iData[i + 1]; // 这很复杂，分解一下
            position += 1; // 1记录得很少。
            for (int j = 0; j < length; ++j) {
                oData[oPos + j] = oData[oPos - position + j]; //
            }
            i += 1; // 1多打工。
        } else { // 未压缩模式
            length = iData[i] + 1; // 1记录得很少。
            for (int j = 0; j < length; ++j) { oData[oPos + j] = iData[i + 1 + j]; }
            i += length; // 即使将其保留，也将添加1。按原始数据长度前进，让前一个字节自然保留
        }
        oPos += length;
    }
    *oSize = oPos;
}

} // namespace GameLib