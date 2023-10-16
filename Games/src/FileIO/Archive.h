#ifndef INCLUDED_GAMELIB_FILEIO_ARCHIVE_H
#define INCLUDED_GAMELIB_FILEIO_ARCHIVE_H

#include "GameLib/Base/HashMap.h"
#include "GameLib/base/Compressor.h"
#include <fstream>
#include <algorithm>
namespace GameLib{
namespace FileIO{
using namespace std;

class Archive{
public:
	Archive( const char* name ) : 
	mFileNumber( 0 ),
	mBlockSize( 0 ),
	mStream( 0 ),
	mDummy( false ){
		if ( !name ){
			mDummy = true; //dummy存档。直接从文件读取。
			return;
		}
		//打开文件并与成员放在一起。
		setlocale( LC_ALL, "" ); //为了处理文件名还有中文字符的文件
		mStream = NEW ifstream( name, ifstream::binary );
		if ( !mStream->good() ){
			cout << "can't open archive: " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		//在末尾前移4个字节
		//是否应该测量一次文件大小？用于错误检查。
		mStream->seekg( 0, ifstream::end );
		streamsize fileSize = mStream->tellg();
		if ( fileSize < 8 ){ //首先
			cout << "archive size is illegal ( too small ): " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		mStream->seekg( -8, ifstream::end );
		mBlockSize = getUnsigned();
		streamsize tableBegin = getUnsigned() * mBlockSize;
		//移至table
		if ( tableBegin + 12 >= fileSize ){ //
			cout << "archive size is illegal ( wrong table ): " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		mStream->seekg( tableBegin, ifstream::beg );
		//4字节读取和文件数
		mFileNumber = getUnsigned();
		if ( fileSize < mFileNumber * 16 ){ //
			cout << "archive size is illegal ( wrong file number ): " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		//分配哈希表
		mEntries.setCapacity( mFileNumber );
		//循环读取
		for ( int i = 0; i < mFileNumber; ++i ){
			Entry e;
			e.mPosition = getUnsigned();
			e.mSize = getUnsigned();
			e.mOriginalSize = getUnsigned();
			e.mNecessaryBufferSize = getUnsigned();
			int nameLength = getUnsigned();
			//名称被临时放置在临时数组中。将尽快删除它。
			char* name = NEW char[ nameLength + 1 ]; //
			mStream->read( name, nameLength );
			name[ nameLength ] = '\0'; //
			//mEntries是map< char*, Entry >
			mEntries.add( name, e ); //储存在map
			//输出信息。对吗
			SAFE_DELETE_ARRAY( name );
		}
	}
	~Archive(){
		SAFE_DELETE( mStream );
	}
	void open( 
	ifstream** streamOut,
	int* entryIndex,
	const char* name ){
		if ( mDummy ){
			*entryIndex = -1;
			setlocale( LC_ALL, "" ); //为了处理文件名还有中文字符的文件
			*streamOut = NEW ifstream( name, ifstream::binary );
			if ( !( ( *streamOut )->good() ) ){
				SAFE_DELETE( *streamOut ); //没有
			}
		}else{
			if ( mFileNumber == 0 ){ //如果没有存档，则总是找不到
				*entryIndex = -1;
				*streamOut = 0;
			}else{
				//
				string tName = name;
				for ( size_t i = 0; i < tName.size(); ++i ){
					if ( tName[ i ] == '/' ){
						tName[ i ] = '\\';
					}
				}
				int it = mEntries.find( tName );
				if ( !mEntries.isEnd( it ) ){
					*entryIndex = it;
					*streamOut = mStream;
					const Entry& e = *mEntries.value( *entryIndex );
					streamsize pos = e.mPosition * mBlockSize;
					mStream->seekg( pos, ifstream::beg );
				}else{
					*entryIndex = -1;
					*streamOut = 0;
				}
			}
		}
	}
	void getFileSize( streamsize* readSize, streamsize* originalSize, int entryIndex, ifstream* stream ) const {
		if ( mDummy ){
			stream->seekg( 0, ifstream::end );
			streamsize r = stream->tellg();
			stream->seekg( 0, ifstream::beg );
			*readSize = *originalSize = r;
		}else{
			STRONG_ASSERT( entryIndex >= 0 );
			STRONG_ASSERT( stream == mStream );
			const Entry& e = *mEntries.value( entryIndex );
			*readSize = e.mSize;
			*originalSize = e.mOriginalSize;
		}
	}
	void allocate( char** dataOut, int size, int entryIndex ){
		if ( mDummy ){
			STRONG_ASSERT( entryIndex == -1 );
			*dataOut = NEW char[ size + 1 ]; //
		}else{
			STRONG_ASSERT( entryIndex >= 0 );
			const Entry& e = *mEntries.value( entryIndex );
			int allocSize = max( e.mNecessaryBufferSize, e.mOriginalSize + 1 ); //如果有扩展所需的缓冲区大小，则将其写入。
			*dataOut = NEW char[ allocSize ];
		}
	}
	void read(
	bool* errorOut,
	char* data,
	int size,
	int entryIndex, 
	ifstream* stream ){
		bool compressed = false;
		int originalSize = size;
		int necessaryBufferSize = size;
		if ( !mDummy ){
			STRONG_ASSERT( stream == mStream );
			STRONG_ASSERT( entryIndex >= 0 );
			const Entry& e = *mEntries.value( entryIndex );
			STRONG_ASSERT( e.mSize == size );
			if ( e.mOriginalSize != size ){
				compressed = true;
				originalSize = e.mOriginalSize;
				necessaryBufferSize = e.mNecessaryBufferSize;
			}
		}
		int readOffset = 0;
		if ( compressed ){
			readOffset = necessaryBufferSize - size; //装在后面。它不是originalSize，因为可能需要额外的扩展空间。
		}
		stream->read( data + readOffset, size );
		if ( stream->gcount() != size ){ //有错吗
			*errorOut = true;
			return;
		}
		if ( compressed ){
			int outSize;
			Compressor::decompress(
				data,
				&outSize,
				data + readOffset,
				size );
			if ( outSize != originalSize ){
				cout << "Decompression Failed!" << endl;
				*errorOut = true;
			}
		}
	}
	void close(	ifstream** stream ){
		if ( mDummy ){
			SAFE_DELETE( *stream );
		}else{
			*stream = 0;
		}
	}
private:
	class Entry{
	public:
		unsigned mPosition;
		int mSize;
		int mOriginalSize;
		int mNecessaryBufferSize;
	};
	unsigned getUnsigned(){
		unsigned char buffer[ 4 ];
		mStream->read( reinterpret_cast< char* >( buffer ), 4 );
		unsigned r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		r |= ( buffer[ 2 ] << 16 );
		r |= ( buffer[ 3 ] << 24 );
		return r;
	}
	int mFileNumber;
	unsigned mBlockSize;
	ifstream* mStream;
	HashMap< string, Entry > mEntries;
	bool mDummy;
};

} //namespace FileIO
} //namespace GameLib

#endif
