#ifndef INCLUDED_GAMELIB_WINDOWCREATOR_H
#define INCLUDED_GAMELIB_WINDOWCREATOR_H

namespace GameLib{

class WindowCreator{
public:
	class Configuration{
	public:
		void setWidth( int width );
		void setHeight( int height );
		void setTitle( const char* title );
		void forbidFullScreen( bool = true ); //调用后alt-enter将被忽略
		void enableFullScreen( bool = true );
		void enableDragAndDrop( bool = true );
	};
	WindowCreator();
	//用户封装函数
	void configure( Configuration* );
	void start( void* windowHandle );
	void update();

	const char* title() const;
	int titleLength() const;
	int width() const;
	int height() const;
	bool isFullScreen() const;
	bool isMinimized() const;
	bool isActive() const;
	void enableFullScreen( bool );
	///获取命令行字符串
	const char* commandLineString() const;
	///拖放有效吗？
	bool isDragAndDropEnabled() const;
	///获取拖放数
	int droppedItemNumber() const;
	///拖放获取
	const char* droppedItem( int index ) const;
	///拖放清除
	void clearDroppedItem();
	///返回毫秒单位时间
	unsigned time() const;

	static bool isMainThread();
	static WindowCreator instance();
	void requestEnd(); //要求结束
	void end(); //发出结束信号。
	bool isEndRequested() const; //需要结束吗
	int getAndResetMouseWheel();
};

} //namespace GameLib

#endif