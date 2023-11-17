这是 [游戏开发：世嘉新人培训教材](https://book.douban.com/subject/34996720/) 书中项目的一个仓库

与原项目不同之处是, 原书是使用 vs2017 的解决方案式来进入管理, 本项目是使用 vscode + cmake 进行管理

# 项目构建流程
## 平台
必须是 windows
## 需要安装
+ [cmake](https://cmake.org/download/)
+ [vscode](https://code.visualstudio.com/download) 插件使用 C/C++ Extension Pack
+ [vs](https://visualstudio.microsoft.com/downloads/)
+ [git](https://git-scm.com/downloads)
## 下载项目
```console
$ git clone https://github.com/v012345/Trial.git
$ cd Trial
$ git submodule init
$ git submodule update
```
## 编译
使用 x86 架构编译, 因为原书项目有一个内存大小检查, 使用 amd64 会编译不通过

## 目录结构
1. Engine 就是原书的底层
1. Example 原书的各个例子
1. Game 我用 lua 和 c/c++ 扩展实现的相似功能
1. extensions lua 功能的扩展
1. libs c/c++ 功能扩展
1. lua lua 源码

## 运行
可执行文件中
+ Game 是运行 lua 脚本的逻辑
+ Example 是运行原书项目的
+ Main 是一个测试新功能的东西

## 其他
# 在编译不通过的情况下, 看看是不是忘了复制一份 `libs\libpng\scripts\pnglibconf.h.prebuilt` => `libs\libpng\pnglibconf.h`

## 说明
说白了, 平山尚先生(原书作者, 下文件尊称为"先生")其实是写了一个游戏引擎, 而我们是在这个引擎的基础上学习游戏开发的"**技术**"与"**技巧**", 但是我发现, 如果完全使用 c++ 学习, 那是一个很痛苦的过程, 还有 vs 也不是一个新手友好的东西, 所以我才做了本项目.

这里我对原项目进行了一个简单的扩展, 前提是不动先生的核心代码, 只在 `Framework.cpp` 文件中注册了一个 lua 方法, 扩展东西我放在了 "extensions" 与 "libs" 中.

## 最后
因为我还在阅读本书, 所以项目也一直会变动, 项目中一定会存在非常多的问题, 我也会一直修复下去的, 直到变成一个"可以看得过去的项目". 如果您有什么疑问可以随机联系我(v138468@gmail.com), 但是最好直接在 github 上的联系我, 因为邮箱里太多广告了, 我一般不看