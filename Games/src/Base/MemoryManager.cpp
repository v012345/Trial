#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#endif

// #include "GameLib/GameLib.h"
//
#include "GameLib/Base/MemoryManager.h"
#include <cstdio>
#include <cstring> //sprintf等，不想使用它，但是没有办法。
#include <locale>

// define STRONG DEBUG//开发调试用的

#ifndef NDEBUG // 仅在调试时调试信息
#define USE_DEBUG_INFO
#endif

// 此文件中不要使用C++的类库
// 原因是，如果在一个名为new的地方执行新操作，则最终会无限递归。
// 很难识别C++库本身的new是哪个版本，因此很难判断要使用哪个库。
// 虽然尽可能不应该使用C函数，但有时绝对需要使用它们。

namespace GameLib {

namespace { // 直到按一定程度稳定运行。因为它很难调试。

using namespace std;

// 类型别名。想缩短代码。
typedef unsigned U4; // 无符号字节别名
typedef unsigned short U2; // 2字节无符号别名

// 设定常数组
const U4 MAX_NORMAL_BLOCK_SIZE = 128 * 1024; // 通过VirtualAlloc直接申请
const U4 HEAP_REGION_SIZE_BIT = 24; // 可以从OS马上获得的区域大小（例如，对于20为1 MB，对于24为16 MB）
const U4 ALIGN = 8; // 可以从大约4,8,16对齐的尺寸中进行选择。选择4时，请确保没有double型。
// 导出常量
const U4 HEAP_REGION_SIZE = (1 << HEAP_REGION_SIZE_BIT);
const U4 TABLE_NUMBER = (HEAP_REGION_SIZE_BIT - 3) * 4; // 空列表箱的数量

// 阻止标志

const U4 FLAG_LARGE_BLOCK = (1 << 0); // 这是一个巨大的障碍。
const U4 FLAG_EMPTY = (1 << 0); // 空。
const U4 FLAG_PREV_EMPTY = (1 << 1); // 前一个块为空
const U4 SIZE_MASK = ~(FLAG_PREV_EMPTY | FLAG_EMPTY);

#ifdef USE_DEBUG_INFO
// 文件名哈希

const U2 FILE_INDEX_MAX = 65521; // 65536最大素数
const U2 FILE_INDEX_UNKNOWN = FILE_INDEX_MAX - 1;
const char* gFileNames[FILE_INDEX_MAX];

U2 getFileNameIndex(const char* p) {
    if (!p) { return FILE_INDEX_UNKNOWN; }
    // 哈希值计算
    ptrdiff_t address = p - static_cast<const char*>(0);
    U2 h = static_cast<U2>(address % FILE_INDEX_MAX);
    // 索引以哈希值作为下标
    for (U2 i = h; i < FILE_INDEX_UNKNOWN; ++i) {
        if (!gFileNames[i] || (gFileNames[i] == p)) {
            gFileNames[i] = p;
            return i;
        }
    }
    // 到这里就相当于结束了。无法以令人满意的速度移动。
    for (U2 i = 0; i < h; ++i) {
        if (!gFileNames[i] || (gFileNames[i] == p)) {
            gFileNames[i] = p;
            return i;
        }
    }
    // STRONG_ASSERT(false && "MemoryManager : FileName Table Full!");
    return FILE_INDEX_UNKNOWN;
}
#endif // DEBUG_INFO

// 获取char*
template <class T> inline char* ptr(T* p) { return reinterpret_cast<char*>(p); }

// 转换回任何类型并返回。
template <class T> inline T* cast(void* p, int offsetInByte) { return reinterpret_cast<T*>(ptr(p) + offsetInByte); }

// 获取地址差异
template <class A, class B> inline U4 diff(A* p0, B* p1) {
    ptrdiff_t t = ptr(p0) - ptr(p1);
    return static_cast<U4>(t);
}

// 对齐指针
template <class T> inline T* align(T* p, U4 n) {
    ptrdiff_t address = ptr(p) - static_cast<char*>(0);
    address += n - 1;
    address &= ~(n - 1);
    return reinterpret_cast<T*>(address);
}

// 对齐整数
inline U4 align(U4 size, U4 n) {
    size += n - 1;
    size &= ~(n - 1);
    return size;
}

#ifdef _WIN32
// 从操作系统获取内存对齐
void* allocateAlignedMemory(void** originalPointer, U4 size, U4 alignSize) {
    // 2预约双地址
    void* reserved = VirtualAlloc(NULL, size * 2, MEM_RESERVE, PAGE_NOACCESS);
    // STRONG_ASSERT(reserved && "MemoryManager : Address Space Full!");
    // 获取适当的对齐地址并从中获取实际区域
    void* aligned = align(reserved, alignSize);
    void* commited = VirtualAlloc(aligned, size, MEM_COMMIT, PAGE_READWRITE);
    // STRONG_ASSERT(commited && "MemoryManager : No Memory!");
    // STRONG_ASSERT((commited == aligned) && "MemoryManager : Unexpected Error! VirtualAlloc() returned invalid value.");

    *originalPointer = reserved; // 返回free的地址
    return commited;
}

// 从操作系统获取内存。无需对齐
void* allocateMemory(U4 size) {
    U4 alignedSize = align(size, 64 * 1024);
    void* p = VirtualAlloc(NULL, alignedSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    // STRONG_ASSERT(p && "MemoryManager : No Memory!");
#ifndef NDEBUG // 填充
    memset(p, 0xfb, size); // 未使用的区域标记
    if (size < alignedSize) { // 禁止区域标记
        memset(ptr(p) + size, 0xf9, alignedSize - size);
    }
#endif
    return p;
}

// 将内存归还操作系统
void deallocateMemory(void* p) {
    BOOL succeeded = VirtualFree(p, 0, MEM_RELEASE);
    // STRONG_ASSERT((succeeded != 0) && "MemoryManager : Deallocation failed! The pointer must be invalid.");
}
#endif

struct Lock {
    Lock() {} // 默认构造函数不执行任何操作。假设全局变量。
    Lock(bool f) { mLock = (f) ? 1 : 0; }
    void lock() {
        // 尝试几次，如果不起作用则切换线程。
        int count = 0;
        while (InterlockedCompareExchange(&mLock, 1, 0) != 0) {
            ++count;
            if (count == 10) { // 10一次左右？
                count = 0;
                Sleep(0);
            }
        }
    }
    bool tryLock() { return (InterlockedCompareExchange(&mLock, 1, 0) == 0); }
    void unlock() { InterlockedCompareExchange(&mLock, 0, mLock); }
    LONG mLock;
};

U4 getBitNumber(U4 a) {
    // 通过二分查找确定位数
    U4 first = 0;
    U4 last = HEAP_REGION_SIZE_BIT;
    while (last >= first + 2) { // 如果差大于或等于2，则请重复
        U4 middle = (first + last) / 2;
        U4 middleV = (1 << middle) - 1;
        if (middleV < a) {
            first = middle;
        } else if (middleV > a) {
            last = middle;
        } else {
            first = last = middle;
        }
    }
    // 差值为0或1，因此返回last。
    return last;
}

inline U4 getHeadIndex(U4 size) {
    U4 r;
    // table
    // 前八个是4i。（0,4,8 ...）。截断。
    // 每四个加倍，例如32、40、48、56。
    if (size <= 32) { // 32到字节为止是线性的。和……并列
        r = size >> 2;
    } else {
        // 首先，确定多少位大小。
        U4 bits = getBitNumber(size);
        // 4bit的数量在[2bit*5，2bit*8）的范围内。
        // 通过查看第12位（即2和4的位置）可以找到8-9、10-11、12-13和14-15的范围。
        // 4bit的数量的话，向右移动1bit，使其变为0-7，观察该下位2bit
        U4 idx = (size >> (bits - 3)) & 0x3;
        r = (bits - 4) * 4 + idx;
    }
    return r;
}

//[Lb]代表LargeBlock
class Heap {
  public:
    Heap(void* reserved) : mReserved(reserved), mNext(0), mLock(false) {
        //
        U4 body = align(sizeof(Heap) + VACANT_HEADER_SIZE - 4, ALIGN) - VACANT_HEADER_SIZE;
        U4 bodySize = HEAP_REGION_SIZE - body - 8; // 将8个字节减少4个字节，强制写入下一个块，然后再写入4倍字节以设置标志。

        // 链接初始化
        for (int i = 0; i < TABLE_NUMBER; ++i) {
            U4 headAddr = diff(mHeads[i], this);
            U4* head = mHeads[i];
            head[PREV_SIZE] = head[SIZE] = 0xffffffff; // 由于没有人看到它，因此请输入似乎会消失的数字。
            head[NEXT] = head[PREV] = headAddr; // 循环
        }
        // 将第一个缓冲区插入列表
        insertToList(body, bodySize);
        setEmptySize(body, bodySize);
        U4 next = body + bodySize;
        setPrevEmptyFlag(next, bodySize);
#ifdef STRONG_DEBUG
        check();
#endif
    }
    static Heap* create() {
        void* reserved;
        Heap* r = reinterpret_cast<Heap*>(allocateAlignedMemory(&reserved, HEAP_REGION_SIZE, HEAP_REGION_SIZE));

        //
        new (r) Heap(reserved);
        return r;
    }
    void removeFromList(U4 addr) {
        if (getSize(addr) >= VACANT_HEADER_SIZE) { // 如果大小不足，则延迟释放。
            U4 next = getNext(addr);
            U4 prev = getPrev(addr);
            setPrev(next, prev);
            setNext(prev, next);
        }
    }
    void insertToList(U4 addr, U4 size) {
        // STRONG_ASSERT(addr < 0xfffff8);
        U4 index = getHeadIndex(size - OCCUPYED_HEADER_SIZE + 4); // 此大小为净值
        // 插入新列表。在开始处插入
        U4 first = mHeads[index][NEXT];
        U4 last = getPrev(first);

        setNext(last, addr);
        setPrev(addr, last);
        setNext(addr, first);
        setPrev(first, addr);
    }

#ifdef USE_DEBUG_INFO
    void* allocate(U4 size, U4 debugInfo) {
#else
    void* allocate(U4 size) {
#endif
#ifdef STRONG_DEBUG
        check();
#endif
        void* r = 0;
        U4 headIndex = getHeadIndex(size) + 1; // 需要一个符合这个尺寸的盒子，因此需要+1
        for (U4 i = headIndex; i < TABLE_NUMBER; ++i) {
            U4 head = diff(mHeads[i], this);
            U4 current = mHeads[i][NEXT];
            if (current != head) {
                U4 currentSizeWithFlag = getSize(current);
                U4 currentSize = currentSizeWithFlag & SIZE_MASK;
                U4 next = current + currentSize;
                // 计算8字节对齐的返回地址（因为+4将破坏下一个标头的前4个字节）
                U4 user = (next + 4 - size) & ~(ALIGN - 1);
                // 从中减去使用的头大小的块头地址
                U4 newBlock = user - OCCUPYED_HEADER_SIZE;
                // 如果大于此块的header，则拆分
                if (newBlock >= current + VACANT_HEADER_SIZE) {
                    // 首先从列表中删除
                    removeFromList(current);
                    // 重设尺寸
                    currentSize = newBlock - current;
                    setEmptySize(current, currentSize);
                    // 用重置大小重新注册到列表
                    insertToList(current, currentSize);
                    // 设置新块
                    setSize(newBlock, next - newBlock);
#ifdef USE_DEBUG_INFO
                    setDebugInfo(newBlock, debugInfo);
#endif
                    // 标志设定
                    setPrevEmptyFlag(newBlock, currentSize); // 之前绝对是空的
                    resetPrevEmptyFlag(next); // 使用下一个
#ifdef STRONG_DEBUG
                    check();
#endif
                } else { // 不会保留。该块按原样使用。
                    removeFromList(current); //
#ifdef USE_DEBUG_INFO
                    setDebugInfo(current, debugInfo);
#endif
                    // 标志设定
                    resetEmptyFlag(current); // 非空
                    resetPrevEmptyFlag(next); // 告知不为空
                    // 返回地址
                    user = current + OCCUPYED_HEADER_SIZE;
#ifdef STRONG_DEBUG
                    check();
#endif
                }
                r = ptr(this) + user;
#ifndef NDEBUG // 调试填充
                memset(r, 0xfb, size); // fb未使用
                char* paddingBegin = ptr(r) + size;
                U4 paddingSize = next + 4 - user - size; //+4超出的部分
                memset(paddingBegin, 0xf9, paddingSize); // 使用禁止区域是f9
#endif
                break; // 确保结束
            }
        }
        return r;
    }
    void deallocate(void* p) {
#ifdef STRONG_DEBUG
        check();
#endif
        U4 addr = diff(p, this) - OCCUPYED_HEADER_SIZE;
        U4 sizeWithFlag = getSize(addr);
        // 删除首先检查它是否可以合并。
        bool prevIsEmpty = ((sizeWithFlag & FLAG_PREV_EMPTY) != 0);
        U4 size = (sizeWithFlag & SIZE_MASK);
        U4 next = addr + size;
#ifndef NDEBUG
        // 附近一致性检查
        if (prevIsEmpty) {
            U4 prevSize = getPrevSize(addr);
            U4 prev = addr - prevSize;
            U4 prevSizeWithFlag = getSize(prev);
            // STRONG_ASSERT(prevSizeWithFlag & FLAG_EMPTY);
            // STRONG_ASSERT(prevSize == (prevSizeWithFlag & SIZE_MASK));
        }
        U4 nextSizeWithFlagDebug = getSize(next);
        // STRONG_ASSERT(!(nextSizeWithFlagDebug & FLAG_PREV_EMPTY)); // 正在用它所以不为空

        // 填充释放标记
        ptrdiff_t fillSize = ptr(this) + next - ptr(p);
        if (fillSize > 0) {
            memset(p, 0xfd, fillSize); // 使用后则fb
        }
#endif
        U4 nextSizeWithFlag = getSize(next);
        bool nextIsEmpty = ((nextSizeWithFlag & FLAG_EMPTY) != 0);
        U4 nextSize = (nextSizeWithFlag & SIZE_MASK);

        if (prevIsEmpty) { // 前面是空的。
            U4 prevSize = getPrevSize(addr);
            U4 prev = addr - prevSize;
            if (nextIsEmpty) { // 下一个也为空
                // 删除一次之前和之后，
                removeFromList(prev);
                removeFromList(next);
                // 调整大小
                prevSize += size + nextSize;
                setEmptySize(prev, prevSize);
                insertToList(prev, prevSize);
                // 告诉下一块它是空的
                U4 nextNext = prev + prevSize;
                setPrevEmptyFlag(nextNext, prevSize);
#ifdef STRONG_DEBUG
                check();
#endif
            } else { // 只有前面空缺
                removeFromList(prev);
                // 调整大小
                prevSize += size;
                setEmptySize(prev, prevSize);
                insertToList(prev, prevSize);
                // 告诉下一块它是空的
                setPrevEmptyFlag(next, prevSize);
#ifdef STRONG_DEBUG
                check();
#endif
            }
        } else {
            if (nextIsEmpty) { // 下一个也为空
                removeFromList(next);
                // 调整大小
                size += nextSize;
                setEmptySize(addr, size);
                insertToList(addr, size);
                // 告诉下一个空的区块的大小
                U4 nextNext = addr + size;
                setPrevSize(nextNext, size);
#ifdef STRONG_DEBUG
                check();
#endif
            } else { // 不粘
                if (size >= VACANT_HEADER_SIZE) { // 仅在可以连接到列表的情况下连接
                    insertToList(addr, size);
                }
                setEmptyFlag(addr);
                // 告诉下一块它是空的
                setPrevEmptyFlag(next, size);
#ifdef STRONG_DEBUG
                check();
#endif
            }
        }
    }
    void check() {

        U4 begin = align(sizeof(Heap) + VACANT_HEADER_SIZE - 4, ALIGN) - VACANT_HEADER_SIZE;
        U4 current = begin;
        U4 prevSizeCheck = 0;
        bool prevIsEmpty = false;
        U4 end = HEAP_REGION_SIZE - OCCUPYED_HEADER_SIZE; // 在更前面。否则不需要头
        while (current < end) {
            U4 sizeWithFlag = getSize(current);
            if (sizeWithFlag & FLAG_PREV_EMPTY) {
                // STRONG_ASSERT(prevIsEmpty);
                U4 prevSize = getPrevSize(current);
                // STRONG_ASSERT(prevSize == prevSizeCheck);
            }
            prevIsEmpty = (sizeWithFlag & FLAG_EMPTY) ? true : false;
            U4 size = sizeWithFlag & SIZE_MASK;
            current += size;
            prevSizeCheck = size;
        }
        for (U4 i = 0; i < TABLE_NUMBER; ++i) {
            U4 head = diff(mHeads[i], this);
            U4 current;
            // 链接升序检查
            current = mHeads[i][NEXT];
            while (current != head) {
                U4 sizeWithFlag = getSize(current);
                // STRONG_ASSERT(sizeWithFlag & FLAG_EMPTY); // 它应该是空的。
                U4 size = sizeWithFlag & SIZE_MASK;
                U4 idx = getHeadIndex(size - OCCUPYED_HEADER_SIZE + 4); // 此大小为净值
                // STRONG_ASSERT(idx == i);
                current = getNext(current);
            }
            // 检查链接降序
            current = mHeads[i][PREV];
            while (current != head) {
                U4 sizeWithFlag = getSize(current);
                // STRONG_ASSERT(sizeWithFlag & FLAG_EMPTY); // 它应该是空的。
                U4 size = sizeWithFlag & SIZE_MASK;
                U4 idx = getHeadIndex(size - OCCUPYED_HEADER_SIZE + 4); // 此大小为净值
                // STRONG_ASSERT(idx == i);
                current = getPrev(current);
            }
        }
    }
    void write(FILE* fp) {
        char str[8192];
        U4 begin = align(sizeof(Heap) + VACANT_HEADER_SIZE - 4, ALIGN) - VACANT_HEADER_SIZE;
        U4 current = begin;
        U4 end = HEAP_REGION_SIZE - OCCUPYED_HEADER_SIZE; // 在更前面。否则不需要头
        while (current < end) {
            U4 sizeWithFlag = getSize(current);
            U4 size = sizeWithFlag & SIZE_MASK;
            if (!(sizeWithFlag & FLAG_EMPTY)) {
#ifdef USE_DEBUG_INFO
                U4 debugInfo = getDebugInfo(current);
                const char* filename = gFileNames[debugInfo & 0xffff];
                U4 line = debugInfo >> 16;
#else
                const char* filename = 0;
                U4 line = 0;
#endif
                if (!filename) { filename = "unknown"; }
                int l = sprintf_s(str, 8192, "%p\t\t%d\t\t%s\t\t%d\n", ptr(this) + current, size, filename, line);
                if (fp) {
                    fwrite(str, 1, l, fp);
                } else {
                    ::OutputDebugStringA(str);
                }
            }
            current += size;
        }
    }
    void lock() { mLock.lock(); }
    bool tryLock() { return mLock.tryLock(); }
    void unlock() { mLock.unlock(); }
    enum Header {
        PREV_SIZE = 0, // 仅当前一个块为空时才使用此选项。在其他时间不可以。
        SIZE = 1,
#ifdef USE_DEBUG_INFO
        DEBUG_INFO = 2, // 接下来是通用的
#endif
        NEXT = 2,
        PREV = 3,

        HEADER_MAX = 4,
    };
#ifdef USE_DEBUG_INFO
    static const U4 OCCUPYED_HEADER_SIZE = 12;
#else
    static const U4 OCCUPYED_HEADER_SIZE = 8;
#endif
    static const U4 VACANT_HEADER_SIZE = 16;
    // header操作功能
    void setNext(U4 addr, U4 target) { (cast<U4>(this, addr))[NEXT] = target; }
    void setPrev(U4 addr, U4 target) { (cast<U4>(this, addr))[PREV] = target; }
    void setSize(U4 addr, U4 size) { (cast<U4>(this, addr))[SIZE] = size; }
    void setEmptySize(U4 addr, U4 size) { (cast<U4>(this, addr))[SIZE] = size | FLAG_EMPTY; }
    void setPrevSize(U4 addr, U4 size) { (cast<U4>(this, addr))[PREV_SIZE] = size; }
    void setPrevEmptyFlag(U4 addr, U4 prevSize) {
        U4* p4 = cast<U4>(this, addr);
        p4[SIZE] |= FLAG_PREV_EMPTY;
        p4[PREV_SIZE] = prevSize;
    }
    void setEmptyFlag(U4 addr) { (cast<U4>(this, addr))[SIZE] |= FLAG_EMPTY; }
    void resetEmptyFlag(U4 addr) { (cast<U4>(this, addr))[SIZE] &= ~FLAG_EMPTY; }
    void resetPrevEmptyFlag(U4 addr) { (cast<U4>(this, addr))[SIZE] &= ~FLAG_PREV_EMPTY; }
    U4 getSize(U4 addr) { return (cast<U4>(this, addr))[SIZE]; }
    U4 getPrevSize(U4 addr) { return (cast<U4>(this, addr))[PREV_SIZE]; }
    U4 getNext(U4 addr) { return (cast<U4>(this, addr))[NEXT]; }
    U4 getPrev(U4 addr) { return (cast<U4>(this, addr))[PREV]; }
#ifdef USE_DEBUG_INFO
    void setDebugInfo(U4 addr, U4 debugInfo) { (cast<U4>(this, addr))[DEBUG_INFO] = debugInfo; }
    U4 getDebugInfo(U4 addr) { return (cast<U4>(this, addr))[DEBUG_INFO]; }
#endif
    void* mReserved; //
    Heap* mNext; // 下一个堆
    U4 mHeads[TABLE_NUMBER][HEADER_MAX]; //
    Lock mLock;
};

class Impl {
  public:
#ifdef USE_DEBUG_INFO
    void* allocate(size_t sizeOrig, U4 debugInfo = FILE_INDEX_UNKNOWN) {
#else
    void* allocate(size_t sizeOrig) {
#endif
#ifdef _WIN64
        // STRONG_ASSERT(sizeoOrig <= 0xffffffff && "allocation over 4GB is forbidden");
#endif
        U4 size = static_cast<U4>(sizeOrig); // 4GB以上的
        // 返回值
        void* r = 0;
        // 大小超出限制。准备专用块
        if (size > MAX_NORMAL_BLOCK_SIZE) {
#ifdef USE_DEBUG_INFO
            r = allocateLb(size, debugInfo);
#else
            r = allocateLb(size);
#endif
        } else {
            // allocate分配堆。
            Heap* current = mHead; // 无需锁。如果为0，则0为好。除0以外的任何值都可以。
            while (current) {
                if (current->tryLock()) {
#ifdef USE_DEBUG_INFO
                    r = current->allocate(size, debugInfo);
#else
                    r = current->allocate(size);
#endif
                    current->unlock();
                }
                if (r) { break; }
                current = current->mNext;
            }

            if (!r) {
                // 没有空间了 必须增加堆。
                Heap* newHeap = Heap::create();
#ifdef USE_DEBUG_INFO
                r = newHeap->allocate(size, debugInfo);
#else
                r = newHeap->allocate(size);
#endif
                // 添加到开头
                mHeapLock.lock();
                Heap* first = mHead;
                mHead = newHeap;
                newHeap->mNext = first;
                mTotalSize += HEAP_REGION_SIZE;
                mHeapLock.unlock();
            }
        }
        // STRONG_ASSERT(r && "BUG!"); // 这是不可能的。这是一个bug
        return r;
    }
    void deallocate(void* p) {
        if (!p) { // 0回避
            return;
        }
        // 如果mNext为0，则为大块。
        if (isLb(p)) {
            deallocateLb(p);
        } else { // 如果不是，请找到Area的开头并将其删除
            Heap* heap = getHeap(p);
            heap->lock();
            heap->deallocate(p);
            heap->unlock();
        }
    }
    void write(const char* filename) {
        // 不能在此使用new。
        // 这意味着无法使用ofstream。因为它是new出来的。
        // 因此，在此文件中，所有操作均由C标准库完成。
        FILE* fp = 0;
        if (filename) {
            setlocale(LC_ALL, "");
            errno_t e = fopen_s(&fp, filename, "w");
            // STRONG_ASSERT((e == 0) && "MemoryManager::write() : can't open output file.");
        } else {
            ::OutputDebugStringA("\n");
        }
        char str[8192]; //
        int l = 0;
        l += sprintf_s(str + l, 8192 - l, "[ MemoryManager::write() ]\n");
        l += sprintf_s(str + l, 8192 - l, "totalSize = %d ( %x ) : %.2fMB\n", mTotalSize, mTotalSize, static_cast<float>(mTotalSize) / 1024.f / 1024.f);
        l += sprintf_s(str + l, 8192 - l, "address\t\tsize\t\tfile\t\tline\n");
        if (fp) {
            fwrite(str, 1, l, fp);
        } else {
            ::OutputDebugStringA(str);
        }

        mLbLock.lock();
        // 首先，从大块
        if (mLbHead) {
            void* begin = mLbHead;
            void* block = begin;
            do {
#ifdef USE_DEBUG_INFO
                U4 debugInfo = getLbDebugInfo(block);
                const char* filename = gFileNames[debugInfo & 0xffff];
                U4 line = debugInfo >> 16;
#else
                const char* filename = 0;
                U4 line = 0;
#endif
                if (!filename) { filename = "unknown"; }
                U4 size = getLbSize(block);
                l = sprintf_s(str, 8192, "%p\t\t%d\t\t%s\t\t%d\n", block, size, filename, line);
                if (fp) {
                    fwrite(str, 1, l, fp);
                } else {
                    ::OutputDebugStringA(str);
                }
                block = getLbNext(block);
            } while (begin != block);
        }
        mLbLock.unlock();
        // 下堆
        mHeapLock.lock();
        Heap* current = mHead;
        while (current) {
            current->write(fp);
            current = current->mNext;
        }
        mHeapLock.unlock();
    }
    void check() {
        checkLb();
        checkHeap();
    }
    ~Impl() {
#ifndef NDEBUG
        write(0);
#endif
    }
    int totalSize() const { return mTotalSize; }

  private:
    void checkLb() {
        mLbLock.lock();
        // 首先，从大块
        if (mLbHead) {
            void* begin = mLbHead;
            void* block = begin;
            do {
                void* next = getLbNext(block);
                void* prev = getLbPrev(block);
                void* nextPrev = getLbPrev(next);
                void* prevNext = getLbNext(prev);
                // STRONG_ASSERT(nextPrev == block);
                // STRONG_ASSERT(prevNext == block);
                block = next;
            } while (begin != block);
        }
        mLbLock.unlock();
    }
    void checkHeap() {
        Heap* h = mHead;
        while (h) {
            h->lock();
            h->check();
            h->unlock();
        }
    }
    Heap* getHeap(void* p) {
        ptrdiff_t address = ptr(p) - static_cast<char*>(0);
        address &= ~(HEAP_REGION_SIZE - 1);
        return reinterpret_cast<Heap*>(address);
    }
#ifdef USE_DEBUG_INFO
    void* allocateLb(U4 size, U4 debugInfo) {
#else
    void* allocateLb(U4 size) {
#endif
#ifdef STRONG_DEBUG
        checkLb();
#endif
        // 使低2bit为0从而对齐
        size = align(size, 8);
        // 标头大小计算。三个指针和两个U4
        U4 alignedHeaderSize = align(LB_HEADER_SIZE, ALIGN);
        void* p = allocateMemory(size + alignedHeaderSize);

        // 返回给用户的指针
        void* r = ptr(p) + alignedHeaderSize;
        setLbSize(r, size);
#ifdef USE_DEBUG_INFO
        setLbDebugInfo(r, debugInfo);
#endif

        // 替换链接
        mLbLock.lock();
        void* prev;
        void* next;
        if (mLbHead) {
            prev = mLbHead;
            next = getLbNext(mLbHead);
            setLbPrev(next, r);
            setLbNext(mLbHead, r);
        } else {
            prev = next = r;
        }
        setLbNext(r, next);
        setLbPrev(r, prev);
        mLbHead = r;
        mTotalSize += size;
        mLbLock.unlock();
#ifdef STRONG_DEBUG
        checkLb();
#endif
        return r;
    }
    void deallocateLb(void* p) {
#ifdef STRONG_DEBUG
        checkLb();
#endif

        // 取消链接
        mLbLock.lock();
        void* prev = getLbPrev(p);
        void* next = getLbNext(p);
#ifndef NDEBUG
        void* nextPrev = getLbPrev(next);
        void* prevNext = getLbNext(prev);
        // STRONG_ASSERT(nextPrev == p);
        // STRONG_ASSERT(prevNext == p);
#endif
        setLbNext(prev, next);
        setLbPrev(next, prev);
        if (mLbHead == p) { // 开始删除了。
            mLbHead = next; // 切换到下一个指针
            if (mLbHead == p) { //
                mLbHead = 0; //
            }
        }
        mTotalSize -= getLbSize(p) & SIZE_MASK;
        mLbLock.unlock();
#ifdef STRONG_DEBUG
        checkLb();
#endif
        // 释放内存
        char* origP = ptr(p) - LB_HEADER_SIZE;
        U4 alignedHeaderSize = align(LB_HEADER_SIZE, ALIGN);
        origP -= alignedHeaderSize - LB_HEADER_SIZE; // 如果对齐了，也要修改返回地址
        deallocateMemory(origP);
    }
/* //
        void* next;
        void* prev;
        U4 size;
        U4 debugInfo; //后续将其返还给用户
*/
#ifdef USE_DEBUG_INFO
    static const int DEBUG_INFO = sizeof(U4);
    static const int SIZE = DEBUG_INFO + sizeof(U4);
#else
    static const int SIZE = sizeof(U4);
#endif
    static const int PREV = SIZE + sizeof(void*);
    static const int NEXT = PREV + sizeof(void*);
    static const int LB_HEADER_SIZE = NEXT;

    static bool isLb(void* p) { // 前4个字节是尺寸值
        U4* up = cast<U4>(p, -SIZE);
        return (*up & FLAG_LARGE_BLOCK);
    }
#ifdef USE_DEBUG_INFO
    static U4 getLbDebugInfo(void* p) { return *cast<U4>(p, -DEBUG_INFO); }
    static void setLbDebugInfo(void* p, U4 di) { *cast<U4>(p, -DEBUG_INFO) = di; }
#endif
    static U4 getLbSize(void* p) { return *cast<U4>(p, -SIZE); }
    static void* getLbPrev(void* p) { return *cast<void*>(p, -PREV); }
    static void* getLbNext(void* p) { return *cast<void*>(p, -NEXT); }
    static void setLbSize(void* p, U4 size) { *cast<U4>(p, -SIZE) = size | FLAG_LARGE_BLOCK; }
    static void setLbPrev(void* p, void* prev) { *cast<void*>(p, -PREV) = prev; }
    static void setLbNext(void* p, void* next) { *cast<void*>(p, -NEXT) = next; }

    // 堆头
    Heap* mHead;
    //
    void* mLbHead;
    //
    Lock mLbLock;
    Lock mHeapLock;
    // 所有尺寸
    int mTotalSize;
};
Impl gImpl;

} // namespace

MemoryManager MemoryManager::instance() { return MemoryManager(); }

void MemoryManager::write(const char* filename) { gImpl.write(filename); }

int MemoryManager::totalSize() const { return gImpl.totalSize(); }

} // namespace GameLib

using namespace GameLib;

#ifdef USE_DEBUG_INFO
void* operator new(size_t size, const char* filename, int line) {
    unsigned debugInfo = (line << 16) | getFileNameIndex(filename);
    return gImpl.allocate(size, debugInfo);
#else
void* operator new(size_t size, const char*, int) {
    return gImpl.allocate(size);
#endif
}

#ifdef USE_DEBUG_INFO
void* operator new[](size_t size, const char* filename, int line) {
    unsigned debugInfo = (line << 16) | getFileNameIndex(filename);
    return gImpl.allocate(size, debugInfo);
#else
void* operator new[](size_t size, const char*, int) {
    return gImpl.allocate(size);
#endif
}

void operator delete(void* p, const char*, int) { gImpl.deallocate(p); }

void operator delete[](void* p, const char*, int) { gImpl.deallocate(p); }

void* operator new(size_t size) { return gImpl.allocate(size); }

void* operator new[](size_t size) { return gImpl.allocate(size); }

void operator delete(void* p) { gImpl.deallocate(p); }

void operator delete[](void* p) { gImpl.deallocate(p); }
