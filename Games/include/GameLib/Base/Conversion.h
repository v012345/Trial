#ifndef INCLUDED_GAMELIB_CONVERSION_H
#define INCLUDED_GAMELIB_CONVERSION_H

// 转换为字符串
// char最多4个字符，short最多6个字符，int最多11个字符，float最多16个字符，double大约30个字符
// 需要传递一个缓冲区。
// 返回值是写入的大小。检查是否超出范围。
namespace GameLib {

int toString10(char* out, char); // 视为整数，而不是字符
int toString10(char* out, unsigned char);
int toString10(char* out, short);
int toString10(char* out, unsigned short);
int toString10(char* out, int);
int toString10(char* out, unsigned);

int toString16(char* out, char); // 视为整数，而不是字符
int toString16(char* out, unsigned char);
int toString16(char* out, short);
int toString16(char* out, unsigned short);
int toString16(char* out, int);
int toString16(char* out, unsigned);

int toString(char* out, float, int precision = 6);
int toString(char* out, double, int precision = 6);

} // namespace GameLib

#endif