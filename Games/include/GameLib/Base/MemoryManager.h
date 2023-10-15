#ifndef INCLUDED_GAMELIB_BASE_MEMORYMANAGER_H
#define INCLUDED_GAMELIB_BASE_MEMORYMANAGER_H

namespace GameLib {

// 控制new的类
class MemoryManager {
  public:
    static MemoryManager instance();

    /// 导出信息（如果提供文件名，则写入到文件，否则将输出到控制台）
    void write(const char* filename = 0);
    /// 确保容量（包括内部预约容量）
    int totalSize() const;
};

} // namespace GameLib

void* operator new(size_t size, const char* filename, int line);
void* operator new[](size_t size, const char* filename, int line);
void operator delete(void*, const char*, int);
void operator delete[](void*, const char*, int);
void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void*);
void operator delete[](void*);

#endif
