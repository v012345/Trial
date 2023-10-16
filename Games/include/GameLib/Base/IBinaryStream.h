#ifndef INCLUDED_GAMELIB_IBINARYSTREAM_H
#define INCLUDED_GAMELIB_IBINARYSTREAM_H

#include "GameLib/Base/RefString.h"

namespace GameLib {

class RefString;

/// 为了方便读取二进制文件的类

class IBinaryStream {
  public:
    IBinaryStream(const char*);
    IBinaryStream(const char*, int size);
    IBinaryStream(const RefString&);
    ~IBinaryStream();
    /// 返回值为读取的字节数
    int read(char*, int size);
    /// 各种内置整数型的版本。成功或者失败会把值传给参数
    int readInt(bool* succeeded = 0);
    unsigned readUnsigned(bool* succeeded = 0);
    short readShort(bool* succeeded = 0);
    unsigned short readUnsignedShort(bool* succeeded = 0);
    char readChar(bool* succeeded = 0);
    unsigned char readUnsignedChar(bool* succeeded = 0);

    bool isEnd() const;
    void setPosition(int = 0);
    int position() const;
    int size() const;

  private:
    const char* mBuffer;
    int mSize;
    int mPosition;
};

} // namespace GameLib

#endif