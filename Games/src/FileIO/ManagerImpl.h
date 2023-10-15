#ifndef INCLUDED_GAMELIB_FILEIO_MANAGERIMPL_H
#define INCLUDED_GAMELIB_FILEIO_MANAGERIMPL_H

#include "GameLib/GameLib.h"
//
#include "FileIO/Archive.h"
#include "FileIO/InFileImpl.h"
#include "FileIO/OutFileImpl.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/Threading/Event.h"
#include "GameLib/Threading/Functions.h"
#include "GameLib/Threading/Mutex.h"
#include "GameLib/Threading/Thread.h"
#include <fstream>
#include <list>
#include <map>
#include <sstream>
#include <string>

namespace GameLib {
namespace FileIO {
using namespace std;
using namespace GameLib::Threading;

class ManagerImpl {
  public:
    class OutRequest {
      public:
        OutRequest(const char* fileName, const char* data, int size, int id, bool isAuto) : mFileName(fileName), mData(0), mSize(size), mId(id), mIsAuto(isAuto) {
            // 整个复制
            mData = NEW char[size];
            for (int i = 0; i < size; ++i) { mData[i] = data[i]; }
        }
        ~OutRequest() {
            SAFE_DELETE_ARRAY(mData);
            mSize = 0;
        }
        int size() const { return mSize; }
        const char* data() const { return mData; }
        const char* fileName() const { return mFileName.c_str(); }
        bool isAuto() const { return mIsAuto; }
        int id() const { return mId; }
        string mFileName;
        char* mData;
        int mSize;
        int mId;
        bool mIsAuto;
    };
    class LoadThread : public Thread {
      public:
        LoadThread(ManagerImpl* manager) : mManager(manager) {}
        ~LoadThread() {
            wait(); // 等待结束
            mManager = 0; // 舍弃变量
        }

      private:
        void operator()() {
            // 无限循环
            while (!mManager->isEndRequested()) { mManager->update(); }
        }
        ManagerImpl* mManager;
    };
    typedef map<string, InFile::Impl*> InMap;
    typedef InMap::iterator InIt;
    typedef InMap::const_iterator CInIt;

    typedef map<int, OutFile::Impl*> OutMap;
    typedef OutMap::iterator OutIt;
    typedef OutMap::const_iterator COutIt;

    typedef list<string> InList;
    typedef list<OutRequest*> OutList;

    ManagerImpl(const char** archiveNames, int archiveNumber, Manager::AccessMode am)
        : mThread(0), mOutRequestId(0), mWarningLimit(0x7fffffff), mErrorLimit(0x7fffffff), mLoadedSum(0), mHaltOnError(true), mArchives(0), mArchiveNumber(0), mAccessMode(am) {
        // 打开档案
        if (mAccessMode == Manager::MODE_ARCHIVE_FIRST || mAccessMode == Manager::MODE_DIRECT_FIRST) {
            mArchiveNumber = archiveNumber + 1;
        } else {
            mArchiveNumber = archiveNumber;
        }
        mArchives = static_cast<Archive*>(OPERATOR_NEW(sizeof(Archive) * mArchiveNumber));

        if (mAccessMode == Manager::MODE_DIRECT_FIRST) {
            // 先放一个dummy
            new (&mArchives[0]) Archive(0); // 直接读取文件
            for (int i = 0; i < archiveNumber; ++i) { new (&mArchives[i + 1]) Archive(archiveNames[i]); }
        } else {
            for (int i = 0; i < archiveNumber; ++i) { new (&mArchives[i]) Archive(archiveNames[i]); }
            if (mAccessMode == Manager::MODE_ARCHIVE_FIRST) {
                new (&mArchives[archiveNumber]) Archive(0); // 直接读取文件
            }
        }
        mMutex = Mutex::create();
        mEndFlag = Event::create();
        mThread = NEW LoadThread(this);
        mThread->start();
    }
    ~ManagerImpl() {
        STRONG_ASSERT(mInFiles.size() == 0 && "FileIO::Manager : you must destroy all files!");
        STRONG_ASSERT(mOutFiles.size() == 0 && "FileIO::Manager : writing files exist. IT MUST BE A BUG.");
        for (OutList::iterator i = mOutRequests.begin(); i != mOutRequests.end(); ++i) {
            if ((*i)->isAuto()) {
                SAFE_DELETE(*i);
            } else {
                HALT("FileIO::Manager : you must wait all write request!");
            }
        }
        mEndFlag.set();
        SAFE_DELETE(mThread); // 删掉

        // 删除存档
        for (int i = 0; i < mArchiveNumber; ++i) { mArchives[i].~Archive(); }
        OPERATOR_DELETE(mArchives);
    }
    InFile::Impl* createInFile(const char* filename) {
        string fn(filename); // 移植到字符串

        mMutex.lock();
        // 首先检查它是否在地图上
        InIt it = mInFiles.find(fn);
        if (it == mInFiles.end()) { // 因为没有所以创建
            InFile::Impl* f = NEW InFile::Impl();
            it = mInFiles.insert(make_pair(fn, f)).first;
            f->setIterator(it);
            // 加载要求
            mInRequests.push_back(fn);
        } else {
            it->second->refer();
        }
        mMutex.unlock();
        return it->second;
    }
    void destroyInFile(InFile::Impl* f) {
        mMutex.lock();
        // 如果这是最后一个，则删除
        f->release();
        if (f->referenceCount() == 0) {
            mLoadedSum -= f->size(); // 总容量减少
            mInFiles.erase(f->iterator());
            SAFE_DELETE(f);
        }
        mMutex.unlock();
    }
    OutFile::Impl* createOutFile(const char* filename, const char* data, int size) {
        mMutex.lock();
        //
        OutFile::Impl* f = NEW OutFile::Impl(mOutRequestId);

        // 生成请求
        OutRequest* req = NEW OutRequest(filename, data, size, mOutRequestId, false);
        //
        mOutFiles.insert(make_pair(mOutRequestId, f));
        mOutRequests.push_back(req);
        ++mOutRequestId; // 准备增加
        mMutex.unlock();

        return f;
    }
    void writeFile(const char* filename, const char* data, int size) {
        mMutex.lock();
        // 生成请求
        OutRequest* req = NEW OutRequest(filename, data, size, mOutRequestId, true);
        ++mOutRequestId; // 准备增加
        // 仅添加请求
        mOutRequests.push_back(req);
        mMutex.unlock();
    }
    void destroyOutFile(OutFile::Impl* f) {
        mMutex.lock();
        // 如果这是最后一个，则删除
        if (f->referenceCount() == 0) {
            OutIt it = mOutFiles.find(f->id());
            STRONG_ASSERT(it != mOutFiles.end());
            mOutFiles.erase(it);
            SAFE_DELETE(f);
        }
        mMutex.unlock();
    }
    void setLimit(int warning, int error) {
        mWarningLimit = warning;
        mErrorLimit = error;
    }
    void enableHaltOnError(bool f) { mHaltOnError = f; }
    void update() {
        using namespace std;
        // 如果不这样做，休眠一会儿
        if (mInRequests.size() == 0 && mOutRequests.size() == 0) {
            Threading::sleep(10);
            return;
        }
        read();
        write();
    }
    void read() {
        ostringstream oss;
        // 获取加载请求
        mMutex.lock();
        if (mInRequests.size() == 0) { // 必须在这里检查
            mMutex.unlock();
            return;
        }
        string fn = mInRequests.front(); // 实物复制。想立即将其从列表中删除
        mInRequests.pop_front();
        mMutex.unlock();

        bool error = false;
        bool notification = false; // 告诉错误处理方？

        // 档案已打开
        int archiveIndex = -1;
        int entryIndex = -1;
        ifstream* stream = 0;
        open(&archiveIndex, &stream, &entryIndex, fn.c_str());

        char* buffer = 0;
        int size = 0;

        if (archiveIndex == -1) { // 找不到文件
            oss.str("");
            oss << "FileIO : can't open file! ( " << fn.c_str() << " )";
            cout << oss.str().c_str() << endl;
            if (mHaltOnError) { HALT("FileIO : can't open file!"); }
            error = notification = true;
        } else {
            // 下一阶段。测量尺寸是否正常。
            streamsize readSize;
            streamsize originalSize;
            mArchives[archiveIndex].getFileSize(&readSize, &originalSize, entryIndex, stream);
            if (originalSize >= 0x80000000) { // int无法存储它！
                oss.str("");
                oss << "FileIO : file is too big! 2GB is maximum. ( " << fn.c_str() << " )";
                cout << oss.str().c_str() << endl;
                if (mHaltOnError) { HALT("FileIO : file is too big! 2GB is maximum."); }
                error = notification = true;
            } else if (mLoadedSum + originalSize > mErrorLimit) { // 已超过限制。
                oss.str("");
                oss << "FileIO : EXCEED ERROR LIMIT! this file is not loaded. ( " << fn.c_str() << " )";
                cout << oss.str().c_str() << endl;
                if (mHaltOnError) { HALT("FileIO : EXCEED ERROR LIMIT! this file is not loaded."); }
                error = notification = true;
            } else if (mLoadedSum + originalSize > mWarningLimit) {
                cout << "FileIO : EXCEED WARNING LIMIT! ( " << fn.c_str() << " )" << endl;
            }

            // 读取之前，请检查是否有处理器
            // 已经加载了吗？
            mMutex.lock();
            InIt it = mInFiles.find(fn);
            if (it == mInFiles.end()) {
                error = true;
            } else {
                if (it->second->isFinished()) { // 结束了！
                    error = true;
                }
            }
            mMutex.unlock();

            if (!error) {
                size = static_cast<int>(readSize);
                // 缓冲区分配
                mArchives[archiveIndex].allocate(&buffer, size, entryIndex);
                // 读取
                bool readError = false;
                mArchives[archiveIndex].read(&readError, buffer, size, entryIndex, stream);
                if (readError) { //
                    oss.str("");
                    oss << "FileIO : read error! can't read entire file! ( " << fn.c_str() << " )";
                    cout << oss.str().c_str() << endl;
                    if (mHaltOnError) { HALT("FileIO : read error! can't read entire file!"); }
                    error = notification = true;
                }
                if (!error) {
                    // 读取完毕
                    size = static_cast<int>(originalSize); //
                    buffer[size] = '\0'; // 可靠的设计。
                }
            }
            if (stream) { mArchives[archiveIndex].close(&stream); }
        }
        // 最终处理
        mMutex.lock();
        InIt it = mInFiles.find(fn); // 再次检查是否存在
        if (it == mInFiles.end()) { // 没有
            error = true;
        }
        if (error) {
            SAFE_DELETE_ARRAY(buffer); // 缓冲区释放
            if (notification) {
                if (it != mInFiles.end()) {
                    it->second->setError(); // 错误标志设置
                }
            }
        } else { // 正常结束
            it->second->set(buffer, size);
            mLoadedSum += size; // 总容量增加
        }
        mMutex.unlock();
    }
    void write() {
        ostringstream oss;
        // 请求处理
        mMutex.lock();
        if (mOutRequests.size() == 0) { // 必须在这里检查
            mMutex.unlock();
            return;
        }
        OutRequest* req = mOutRequests.front();
        mOutRequests.pop_front();
        mMutex.unlock();

        bool isAuto = req->isAuto();
        int id = req->id();
        // 档案已打开
        const char* fileName = req->fileName();
        setlocale(LC_ALL, ""); // 为了处理文件名还有中文字符的文件
        ofstream out(fileName, ofstream::binary);
        bool error = false;
        if (!out) {
            oss.str("");
            oss << "FileIO : can't open file! ( " << fileName << " )";
            cout << oss.str().c_str() << endl;
            if (mHaltOnError) { HALT("FileIO : can't open file!"); }
            error = true;
        } else {
            // 取出数据
            int size = req->size();
            const char* data = req->data();
            const char* readPos = data;
            static const int WRITE_UNIT = 1024 * 1024; // 1MB
            int rest = size;
            while (!error) {
                int writeSize = min(rest, WRITE_UNIT);
                out.write(readPos, writeSize);
                readPos += writeSize;
                rest -= writeSize;
                STRONG_ASSERT(rest >= 0); //
                if (rest == 0) {
                    break; // 正常结束
                }
                // 真的写入了吗？
                streamsize wroteSize = out.tellp();
                if (wroteSize != (size - rest)) {
                    oss.str("");
                    oss << "FileIO : write error! can't write entire file! ( " << fileName << " )";
                    cout << oss.str().c_str() << endl;
                    if (mHaltOnError) { STRONG_ASSERT(0 && "FileIO : write error! can't write entire file!"); }
                    error = true;
                    break;
                }
                // 处理删除检查（仅当应该有一个处理时）
                if (!isAuto) {
                    mMutex.lock();
                    OutIt it = mOutFiles.find(id);
                    if (it == mOutFiles.end()) { //
                        error = true;
                    }
                    mMutex.unlock();
                }
            }
        }
        // 结果通知
        if (!isAuto) {
            mMutex.lock();
            OutIt it = mOutFiles.find(id);
            if (it != mOutFiles.end()) { //
                it->second->setFinished();
                if (error) { it->second->setError(); }
            }
            mMutex.unlock();
        }
        SAFE_DELETE(req); // 销毁要求
    }
    bool isEndRequested() const { return mEndFlag.isSet(); }
    string createListString() const {
        ostringstream oss;
        CInIt i = mInFiles.begin();
        while (i != mInFiles.end()) {
            oss << i->first;
            oss << '\t' << i->second->size();
            oss << '\t' << i->second->referenceCount() << '\n';
            ++i;
        }
        string ret = oss.str();
        return ret;
    }
    void open(int* archiveIndex, ifstream** streamOut, int* entryIndex, const char* name) {
        for (int i = 0; i < mArchiveNumber; ++i) {
            *archiveIndex = i;
            mArchives[i].open(streamOut, entryIndex, name);
            if (*streamOut) { // 找到了 退出
                break;
            }
        }
        if (!(*streamOut)) {
            *archiveIndex = -1; // 最终没有找到。
        }
    }
    Mutex mMutex;
    LoadThread* mThread;
    Event mEndFlag;
    int mOutRequestId;

    InMap mInFiles;
    OutMap mOutFiles;
    InList mInRequests;
    OutList mOutRequests;

    int mWarningLimit;
    int mErrorLimit;
    int mLoadedSum;
    bool mHaltOnError;

    Archive* mArchives;
    int mArchiveNumber;
    Manager::AccessMode mAccessMode;
};
extern ManagerImpl* gManagerImpl;

} // namespace FileIO
} // namespace GameLib

#endif