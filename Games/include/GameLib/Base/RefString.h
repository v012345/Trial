#ifndef INCLUDED_GAMELIB_REFSTRING_H
#define INCLUDED_GAMELIB_REFSTRING_H

// #include <string>

namespace GameLib {
// using namespace std;

// 引用类型字符串类。

class RefString {
  public:
    RefString();
    RefString(const char*, int size);
    explicit RefString(const char*);
    ~RefString();
    void set(const char*, int size);
    void operator=(const char*);
    // void operator=( const string& );
    char operator[](int i) const;
    const char* get() const;
    int size() const;
    void setSize(int);
    // last是最后一个文字。不要+1。last==-1只改变起点
    void clamp(int first, int last = -1);
    int find(char) const; // 从前面查找字符并返回位置。-1（如果找不到）。
    int rfind(char) const; // 稍后找到角色并返回位置。-1（如果找不到）。
    /// 复制引用目标字符串并返回指针。自己delete
    char* copyOriginal() const;

    bool operator==(const RefString&) const;
    bool operator==(const char*) const;
    // bool operator==( const string& ) const;
    bool operator!=(const RefString&) const;
    bool operator!=(const char*) const;
    // bool operator!=( const string& ) const;
    bool operator<(const RefString&) const;

  private:
    const char* mString;
    int mSize;
};

} // namespace GameLib

#endif