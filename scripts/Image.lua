--- 对应 Image.cpp
require "Object"
Image = {}

setmetatable(Image, {
    __call = function(self, filename)
        local obj = {}
        function obj:isA(what)
            return Image == what
        end

        function obj:blend(src, dst)
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

        ---comment
        ---@param dstX integer|nil
        ---@param dstY integer|nil
        ---@param srcX integer|nil
        ---@param srcY integer|nil
        ---@param width integer|nil
        ---@param height integer|nil
        function obj:draw(dstX, dstY, srcX, srcY, width, height)
            dstX = dstX or 1
            dstY = dstY or 1
            srcX = srcX or 1
            srcY = srcY or 1
            width = width or self.mWidth
            height = height or self.mHeight
            for y = 0, height - 1 do
                for x = 0, width - 1 do
                    local src = self.mData[y + srcY][x + srcX]
                    local dst = Screen:colorAt(x + dstX, y + dstY)
                    local color = self:blend(src, dst)
                    Screen:setColor(x + dstX, y + dstY, color)
                end
            end
        end

        local image = ReadPngFile(filename)
        obj.mHeight = #image
        obj.mWidth = #image[1]
        obj.mData = image
        setmetatable(obj, {
            __index = Object(),
        })
        return obj
    end
})
