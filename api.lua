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

Framework = {}
---返回时间毫秒
---@return integer
function Framework:time() return 0 end

---休息多少毫秒
---@param ms integer
function Framework:sleep(ms) end

---是否按了某个键
---@param c integer
---@return boolean
function Framework:isKeyOn(c) return true end

function Framework:frameRate() return 0 end

function Framework:setFrameRate(fps) end

---是否按了某个键(一次)
---@param c integer
---@return boolean
function Framework:isKeyTriggered(c) return true end

---在窗口输出 debug 信息
---@param c integer
---@param r integer
---@param text string
---@param color integer|nil
function Framework:drawDebugString(c, r, text, color) end

function Framework:mouse() return {} end

---@deprecated
function Framework:setVideoMemory(x, y, c) return {} end

function Framework:width() return 0 end

function Framework:height() return 0 end

function Framework:drawTriangle2D(p1, p2, p3, t1, t2, t3, c1, c2, c3) end

---comment
---@param filename any
---@return userdata
function Framework:createTexture(filename)
    return self:createTexture("")
end

---@param texture userdata
function Framework:setTexture(texture) end

function Framework:setBlendMode(mode) end

function Framework:drawTriangle3D(p1, p2, p3, t1, t2, t3, c1, c2, c3) end

function Framework:enableDepthTest(b) end

function Framework:enableDepthWrite(b) end

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

    function obj:pixel(x, y) return 0 end

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
---@return XML
function XML(path) return {} end

---@param path string|nil
---@return CSV
function CSV(path) return {} end

---@param path string|nil
---@return JSON
function JSON(path) return {} end

---@param x any
---@param y any
---@return Vector2
function Vector2(x, y) return {} end

---@param e00 any
---@param e01 any
---@param e10 any
---@param e11 any
---@return Matrix22
function Matrix22(e00, e01, e10, e11) return {} end

---@param e00 any
---@param e01 any
---@param e10 any
---@param e11 any
---@return Matrix23
function Matrix23(e00, e01, e10, e11) return {} end
