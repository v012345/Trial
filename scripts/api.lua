---设置终端游标位置
---@param column integer
---@param line integer
function SetConsoleCursorPosition(column, line) end

---立即从终端获取输入
---@return integer
---@diagnostic disable-next-line
function _getch() return 0 end

---源码目录
CMAKE_SOURCE_DIR = "${CMAKE_SOURCE_DIR}/"

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
---@return table<table<integer>>
function ReadPngFile(path)
    ---@type table<table<integer>>
    local image = { { 1 } }
    return image
end

Framework = {}
---休息多少毫秒
---@param ms integer
function Framework.sleep(ms) end

---返回时间毫秒
---@return integer
function Framework.time() return 0 end

---是否按了某个键
---@param c integer
---@return boolean
function Framework.isKeyOn(c) return true end

---游戏使用到的数据
Game = {
    -- 前一帧时间
    PreviousTime = {},
    --背景
    background = {}
}
---@class Image
Image = {
    { 0xffff0000, 0xffff0000, 0xffff0000 },
    { 0xffff0000, 0xffff0000, 0xffff0000 },
    { 0xffff0000, 0xffff0000, 0xffff0000 },
}

---@class StageData
StageData = {
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 }
}
