---comment
local NimotsuKunImags = ReadPngFile(CMAKE_SOURCE_DIR .. "/res/nimotsuKunImageAlpha.png")
---@param IMAGE image
local function drawImage(IMAGE, atX, atY)
    for i = 1, IMAGE.height, 1 do
        for j = 1, IMAGE.width, 1 do
            local argb = IMAGE.ARGB[(i - 1) * IMAGE.width + j]
            local srcA = (argb & 0xff000000) >> 24
            local srcR = argb & 0xff0000;
            local srcG = argb & 0x00ff00;
            local srcB = argb & 0x0000ff;
            local dst = 0
            local dstR = dst & 0xff0000;
            local dstG = dst & 0x00ff00;
            local dstB = dst & 0x0000ff;
            -- srcA = 255 - srcA
            local r = (srcR - dstR) * srcA // 255 + dstR;
            local g = (srcG - dstG) * srcA // 255 + dstG;
            local b = (srcB - dstB) * srcA // 255 + dstB;
            -- 如果大于255则设置为255
            r = (r > 0xff0000) and 0xff0000 or r;
            g = (g > 0x00ff00) and 0x00ff00 or g;
            b = (b > 0x0000ff) and 0x0000ff or b;
            dst = (r & 0xff0000) | (g & 0x00ff00) | b;

            -- Impl:vram(atX + i - 1, atY + j - 1, srcA + (srcA << 8) + (srcA << 16))
            Impl:vram(atX + i - 1, atY + j - 1, dst)
        end
    end
end
drawImage(NimotsuKunImags, 0, 0)
---@param Stages table<table<integer>>
local function ConertStagesToImage(Stages)

end
