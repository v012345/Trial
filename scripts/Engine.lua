---在 (atX,atY) 画 what
---@param what Image
---@param atX integer
---@param atY integer
---@param isBackground boolean
function Framework:draw(what, atX, atY, isBackground)
    for y, row in ipairs(what) do
        for x, argb in ipairs(row) do
            if isBackground then
                Impl:vram(atX + x - 1, atY + y - 1, argb)
            else
                local srcA = (argb & 0xff000000) >> 24
                local srcR = argb & 0xff0000;
                local srcG = argb & 0x00ff00;
                local srcB = argb & 0x0000ff;
                local dst = Impl:colorAt(atX + x - 1, atY + y - 1)
                local dstR = dst & 0xff0000;
                local dstG = dst & 0x00ff00;
                local dstB = dst & 0x0000ff;
                local r = (srcR - dstR) * srcA // 255 + dstR;
                local g = (srcG - dstG) * srcA // 255 + dstG;
                local b = (srcB - dstB) * srcA // 255 + dstB;
                -- 如果大于255则设置为255
                r = (r > 0xff0000) and 0xff0000 or r;
                g = (g > 0x00ff00) and 0x00ff00 or g;
                b = (b > 0x0000ff) and 0x0000ff or b;
                dst = (r & 0xff0000) | (g & 0x00ff00) | b;
                Impl:vram(atX + x - 1, atY + y - 1, dst)
            end
        end
    end
end
