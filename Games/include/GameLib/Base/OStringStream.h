#ifndef INCLUDED_GAMELIB_OSTRINGSTREAM_H
#define INCLUDED_GAMELIB_OSTRINGSTREAM_H

#include <string>

namespace GameLib {
using namespace std;

class RefString;

// 用于替换std::ostringstream。为了性能可以牺牲一些功能

class OStringStream {
  public:
    OStringStream(int blockSize = 1024); // 一次new的容量，值越大，负载越轻，但是消耗的内存越多。
    ~OStringStream();
    int precision(int);
    OStringStream& operator<<(char);
    OStringStream& operator<<(unsigned char);
    OStringStream& operator<<(int);
    OStringStream& operator<<(unsigned);
    OStringStream& operator<<(short);
    OStringStream& operator<<(unsigned short);
    OStringStream& operator<<(float);
    OStringStream& operator<<(double);
    OStringStream& operator<<(const string&);
    OStringStream& operator<<(const char*);
    OStringStream& operator<<(char*);
    OStringStream& operator<<(const RefString&);
    OStringStream& operator<<(OStringStream& (*f)(OStringStream&));
    void write(const char*, int size);
    template <class T> void write(const T&); // 写入二进制
    OStringStream& endl();
    OStringStream& hex();
    OStringStream& dec();

    void get(Array<char>*) const;
    void get(string*) const;
    int size() const;
    void clear();

  private:
    OStringStream(const OStringStream&); // 禁止复制构造函数
    void operator=(const OStringStream&); // 禁止赋值
    class Impl;
    Impl* mImpl;
};

OStringStream& endl(OStringStream&);
OStringStream& hex(OStringStream&);
OStringStream& dec(OStringStream&);

} // namespace GameLib
#include "GameLib/Base/Impl/OStringStreamImpl.h"

#endif
