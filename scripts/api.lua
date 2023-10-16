---设置终端游标位置
---@param column integer
---@param line integer
function SetConsoleCursorPosition(column, line) end

---立即从终端获取输入
---@return integer
---@diagnostic disable-next-line
function _getch() return 0 end

CMAKE_SOURCE_DIR = "源码目录"

Impl = {}
---在窗口的指定位置画一个点
---@param w integer 第几行
---@param h integer 第几列
---@param c integer 什么颜色
function Impl:vram(w, h, c) end

---窗口的高
---@return integer
function Impl:height() return 0 end

---窗口的宽
---@return integer
function Impl:width() return 0 end
