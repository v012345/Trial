#include "GameLib/Base/Conversion.h"

namespace GameLib {

namespace { // 匿名了, 其他文件不可以访问, 只有内部文件可以访问

int toString16(char* out, unsigned a, int n) { // n是16演变时的最大位数
    static const char table[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    };
    for (int i = 0; i < n; ++i) { // 从高位开始按顺序处理
        int shift = (n - 1 - i) * 4;
        unsigned mask = 0xf << shift;
        unsigned t = a & mask;
        t >>= shift;
        out[i] = table[t];
    }
    return n;
}

// 不带前导零的返回。
void toString10Core(char* out, unsigned a, int n) { // n是10演变时的最大位数。最多10
    // 10进制常数
    static const int d[9] = {
        10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
    };
    // 10将除以10的余数保存起来。
    for (int i = 0; i < n - 1; ++i) {
        int q = a / d[n - 2 - i];
        out[i] = static_cast<char>(q);
        a -= q * d[n - 2 - i];
    }
    out[n - 1] = static_cast<char>(a);
}

int toString10(char* out, unsigned a, int n, bool minus) { // n是10演变时的最大位数。最多10
    char* p = out;
    // 如果为负，则输出负
    if (minus) { *p++ = '-'; }
    char s[10]; // 中间缓冲
    toString10Core(s, a, n);
    // 忽略0
    int begin = n;
    for (int i = 0; i < n; ++i) {
        if (s[i] != 0) {
            begin = i;
            break;
        }
    }
    if (begin == n) { // 一位数字都没有。就是0
        *p++ = '0';
    } else {
        for (int i = begin; i < n; ++i) { *p++ = '0' + s[i]; }
    }
    return static_cast<int>(p - out);
}

} // namespace

int toString10(char* out, char a) {
    bool minus = false;
    if (a < 0) {
        minus = true;
        a = -a;
    }
    return toString10(out, a, 3, minus);
}

int toString10(char* out, unsigned char a) { return toString10(out, a, 3, false); }

int toString10(char* out, short a) {
    bool minus = false;
    if (a < 0) {
        minus = true;
        a = -a;
    }
    return toString10(out, a, 5, minus);
}

int toString10(char* out, unsigned short a) { return toString10(out, a, 5, false); }

int toString10(char* out, int a) {
    bool minus = false;
    if (a < 0) {
        minus = true;
        a = -a;
    }
    return toString10(out, a, 10, minus);
}

int toString10(char* out, unsigned a) { return toString10(out, a, 10, false); }

int toString16(char* out, char a) { return toString16(out, a, 2); }

int toString16(char* out, unsigned char a) { return toString16(out, a, 2); }

int toString16(char* out, short a) { return toString16(out, a, 4); }

int toString16(char* out, unsigned short a) { return toString16(out, a, 4); }

int toString16(char* out, int a) { return toString16(out, a, 8); }

int toString16(char* out, unsigned a) { return toString16(out, a, 8); }

int toString(char* out, float a, int precision) {
    char* p = out; // 写指针
    if (a < 0.f) { // 符号处理
        *p++ = '-';
        a = -a;
    }
    unsigned o = *reinterpret_cast<unsigned*>(&a);
    int e = (o & 0x7f800000) >> 23;
    unsigned m = o & 0x007fffff; // 给隐藏bit加1
    // 处理特殊数
    if (e == 0) {
        *p++ = '0';
        return static_cast<int>(p - out);
    } else if (e == 255) {
        if (m == 0) {
            *p++ = 'I';
            *p++ = 'n';
            *p++ = 'f';
        } else {
            *p++ = 'N';
            *p++ = 'a';
            *p++ = 'N';
        }
        return static_cast<int>(p - out);
    }
    e -= 127; // 指数偏差
    e -= 23; // 从指数中减去2 ^ 23位尾数
    m += 0x00800000; // 给隐藏bit加1
    m <<= 8; // 8位移位
    e -= 8; //

    int e10 = 0;
    if (e > 0) { // 2乘幂十除
        for (int i = 0; i < e; ++i) {
            if (m > 0x80000000) { // 溢出！
                m /= 10;
                ++e10;
                if (i + 2 < e) { //
                    m <<= 2;
                    i += 2;
                }
            }
            m <<= 1;
        }
    } else {
        e = -e;
        for (int i = 0; i < e; ++i) {
            if (m < 0x19000000) { // 提升位数
                m *= 10;
                --e10;
                if (i + 2 < e) { //
                    m >>= 2;
                    i += 2;
                }
            }
            m >>= 1;
        }
    }
    char s[10]; // 中间输出缓冲
    toString10Core(s, m, 10);
    // 下面的格式

    // 移位以使开头不为0。简化后续步骤
    int zeroEnd = 0;
    for (int i = 0; i < 10; ++i) {
        if (s[i] != 0) {
            break;
        } else {
            ++zeroEnd;
        }
    }
    if (zeroEnd == 10) { // 都是0种特殊情况
        *p++ = '0';
        return static_cast<int>(p - out);
    }
    // 移动
    for (int i = 0; i < 10 - zeroEnd; ++i) { s[i] = s[i + zeroEnd]; }
    for (int i = 10 - zeroEnd; i < 10; ++i) { s[i] = 0; }
    e10 -= zeroEnd; //
    e10 += 9; // 10亿位为1，增加到9位数

    // 输出位数
    int n = (precision < 9) ? precision : 9;
    // 四舍五入
    bool carry = (s[n] >= 5);
    s[n] = 0;
    if (carry) {
        for (int i = n - 1; i >= 0; --i) {
            ++s[i];
            if (s[i] == 10) {
                s[i] = 0;
            } else {
                carry = false;
                break;
            }
        }
        if (carry) { // 它上升到最大位数。
            s[0] = 1;
            for (int i = 1; i < 10; ++i) { s[i] = 0; }
            ++e10;
        }
    }
    //
    for (int i = n - 1; i > 0; --i) {
        if (s[i] == 0) {
            --n;
        } else {
            break;
        }
    }
    // 如果e10为[-2，precision），则不使用e表示法。因为它更短。
    if ((e10 >= -2) && (e10 < precision)) {
        if (e10 >= 0) { // 当第一个数字是整数时
            for (int i = 0; i < n; ++i) {
                *p++ = '0' + s[i];
                if ((i == e10) && (i != n - 1)) { //
                    *p++ = '.'; // 小数点输出
                }
            }
            //
            for (int i = n; i <= e10; ++i) { *p++ = '0'; }
        } else { // 当第一位是小数时
            *p++ = '0';
            *p++ = '.';
            if (e10 == -2) { *p++ = '0'; }
            for (int i = 0; i < n; ++i) { *p++ = '0' + s[i]; }
        }
    } else { // e表示法
        // 输出一位
        *p++ = '0' + s[0];
        // 小数点输出
        *p++ = '.';
        for (int i = 1; i < n; ++i) { *p++ = '0' + s[i]; }
        if (e10 != 0) {
            *p++ = 'e';
            if (e10 < 0) {
                *p++ = '-';
                e10 = -e10;
            }
            int q = e10 / 10;
            if (q != 0) { *p++ = '0' + static_cast<char>(q); }
            e10 -= q * 10;
            *p++ = '0' + static_cast<char>(e10);
        }
    }
    return static_cast<int>(p - out);
}

int toString(char* out, double a, int precision) {
    return toString(out, static_cast<float>(a), precision); // TODO
}

} // namespace GameLib
