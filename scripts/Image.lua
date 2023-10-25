--- 对应 Image.cpp
Image = {}
local function blend(src, dst)
    local srcA = (src & 0xff000000) >> 24;
    local srcR = src & 0xff0000;
    local srcG = src & 0x00ff00;
    local srcB = src & 0x0000ff;
    local dstR = dst & 0xff0000;
    local dstG = dst & 0x00ff00;
    local dstB = dst & 0x0000ff;
    local r = (srcR - dstR) * srcA // 255 + dstR;
    local g = (srcG - dstG) * srcA // 255 + dstG;
    local b = (srcB - dstB) * srcA // 255 + dstB;
    return (r & 0xff0000) | (g & 0x00ff00) | b;
end



---@class Image
local mt = {}

---@param filename string
---@return Image
function Image:new(filename)
    ---@class Image
    local obj = {}
    setmetatable(obj, { __index = mt })
    local image = ReadPngFile(filename)
    obj.iHeight = #image
    obj.iWidth = #image[1]
    obj.tData = image
    return obj
end

---comment
---@param dstX integer|nil
---@param dstY integer|nil
---@param srcX integer|nil
---@param srcY integer|nil
---@param width integer|nil
---@param height integer|nil
function mt:draw(dstX, dstY, srcX, srcY, width, height)
    dstX = dstX or 0
    dstY = dstY or 0
    srcX = srcX or 0
    srcY = srcY or 0
    width = width or self.iWidth
    height = height or self.iHeight
    for y = 1, height do
        for x = 1, width do
            local src = self.tData[y][x]
            local dst = Screen:colorAt(x, y)
            local color = blend(src, dst)
            Screen:setColor(x, y, color)
        end
    end
end

function mt:destroy()
end
