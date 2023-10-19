#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void displayHelp();
void write(bool* succeeded, ofstream&, const char* inFileName);

int main(int argc, const char** argv) {
    vector<const char*> inFiles;
    const char* outFile = 0;
#ifdef CMAKE_SOURCE_DIR
    // 直接在 CMAKE 里定义, 省得传参
    inFiles.push_back(CMAKE_SOURCE_DIR "GameLib/font.tga");
    outFile = CMAKE_SOURCE_DIR "GameLib/fontTextureGenerated.h";
#else
    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (arg[0] == '-') {
            if (arg[1] == 'o') {
                if (i + 1 < argc) {
                    outFile = argv[i + 1];
                    ++i;
                }
            } else if (arg[1] == 'h') {
                displayHelp();
                return 0;
            }
        } else {
            inFiles.push_back(arg);
        }
    }
#endif

    if (inFiles.size() == 0 || !outFile) {
        displayHelp();
        return 0;
    }
    // 打开输出文件
    setlocale(LC_ALL, ""); // 确保可以处理文件名还有中文的情况
    ofstream o(outFile, ofstream::binary);
    if (!o) {
        cerr << "can't open output file." << endl;
        return 1;
    }
    // 读取文件并且进行处理
    bool succeeded;
    for (unsigned i = 0; i < inFiles.size(); ++i) {
        write(&succeeded, o, inFiles[i]);
        if (!succeeded) {
            cerr << "can't open/read input file." << endl;
            cerr << inFiles[i] << endl;
            return 2;
        }
    }
}

void write(bool* succeeded, ofstream& o, const char* inFileName) {
    setlocale(LC_ALL, ""); // 为了确保可以对包含中文字符的文件名进行处理
    ifstream in(inFileName, ifstream::binary);
    if (!in) {
        *succeeded = false;
        return;
    }
    // 一次性读入
    in.seekg(0, ifstream::end);
    int s = static_cast<int>(in.tellg());
    in.seekg(0, ifstream::beg);
    char* b = new char[s];
    in.read(b, s);
    int readSize = static_cast<int>(in.gcount());
    if (readSize != s) {
        *succeeded = false;
        return;
    }
    // 根据文件名生成变量
    // 忽略斜线，反斜线，点好以及0x80以上的字符，剩下字符都转换成大写
    string name;
    bool capital = true;
    for (unsigned i = 0; inFileName[i]; ++i) {
        if (inFileName[i] == '/') {
            capital = true;
        } else if (inFileName[i] == '\\') {
            capital = true;
        } else if (inFileName[i] == '.') {
            capital = true;
        } else if (!isgraph(inFileName[i])) {
            capital = true;
        } else {
            char c = inFileName[i];
            if (capital) {
                c = toupper(c);
                capital = false;
            }
            name += c;
        }
    }
#ifdef CMAKE_SOURCE_DIR
    // 直接在 CMAKE 里定义, 省得传参
    o << "namespace {\n";
    o << "const char g"
      << "FontTga"
      << "[] = {\\\n";
#else
    // 开始写入head
    o << "namespace {\n";
    o << "const char g" << name << "[] = {\\\n";
#endif

    // 循环
    int rowCount = 0;
    for (streamsize i = 0; i < s; ++i) {
        o << static_cast<int>(b[i]) << ", ";
        ++rowCount;
        if (rowCount == 16) {
            o << '\\' << '\n';
            rowCount = 0;
        }
    }
    // 最后加换行
    o << '\\' << '\n';
    // 反括号
    o << "};\n";
    o << "} //anonymous namespace\n";
    *succeeded = true;
}

void displayHelp() {
    cout << "convert contents of files to C constant string" << endl;
    cout << "usage : ConvertToCode.exe -o OUTFILE INFIle..." << endl;
}
