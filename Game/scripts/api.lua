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
CMAKE_CURRENT_SOURCE_DIR = "${CMAKE_CURRENT_SOURCE_DIR}/"

Screen = {}
---在窗口的指定位置画一个点, lua 里的 (1,100) 对 c 里的 (0,99)
---@param x integer
---@param y integer
---@param color integer
function Screen:setColor(x, y, color) end

---窗口的高
---@return integer
function Screen:height() return 0 end

---窗口的宽
---@return integer
function Screen:width() return 0 end

---返回指定点的颜色, lua 里的 (1,100) 对 c 里的 (0,99)
---@return integer
function Screen:colorAt(x, y) return 0 end

---清空屏幕
function Screen:clear() end

---在 c 里解析 png 图片
---@param path string png 图片的路径
---@return Image
function ReadPngFile(path) return {} end

Framework = {}
---休息多少毫秒
---@param ms integer
function Framework:sleep(ms) end

---返回时间毫秒
---@return integer
function Framework:time() return 0 end

function Framework:frameRate() return 0 end

---是否按了某个键
---@param c integer
---@return boolean
function Framework:isKeyOn(c) return true end

---在窗口输出 debug 信息
---@param c integer
---@param r integer
---@param text string
---@param color integer|nil
function Framework:drawDebugString(c, r, text, color) end

---是否按了某个键(一次)
---@param c integer
---@return boolean
function Framework:isKeyTriggered(c) return true end

function Framework:setFrameRate(fps) end

---comment
---@param char integer unicode
---@param width integer
---@param height integer
---@param color integer
---@param font string
function GetFontBmp(char, width, height, color, font)
end

StringRenderer = {}

---comment
---@param x integer|nil
---@param y integer|nil
---@param s string|nil
---@param c integer|nil
function StringRenderer:draw(x, y, s, c)

end

---C 方法
---@param path string
---@return Image
function Image(path)
    ---@class Image
    local obj = {}
    function obj:height() return 0 end

    function obj:width() return 0 end

    ---@param dstX integer|nil
    ---@param dstY integer|nil
    ---@param srcX integer|nil
    ---@param srcY integer|nil
    ---@param width integer|nil
    ---@param height integer|nil
    function obj:draw(dstX, dstY, srcX, srcY, width, height) end

    return obj
end

---comment
---@param path string
---@return ParseXML
function ParseXML(path) return {} end

---@param path string|nil
---@return CSV
function CSV(path) return {} end
