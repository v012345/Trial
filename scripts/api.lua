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

---返回指定点的颜色
---@return integer
function Impl:colorAt(x, y) return 0 end

---清空屏幕
function Impl:clear() end

---在 c 里解析 png 图片
---@param path string png 图片的路径
---@return image
function ReadPngFile(path)
    ---@class image
    ---@field ARGB table<integer>
    local image = { width = 1, height = 1, ARGB = { 1 } }
    return image
end

Framework = {}
---休息多少毫秒
---@param ms integer
function Framework.sleep(ms) end
