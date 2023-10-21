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

---在 (atX,atY) 写 msg
---@param msg string
---@param atX integer
---@param atY integer
---@param width integer|nil
---@param height integer|nil
---@param color integer|nil
---@param font string|nil
function Framework:string(msg, atX, atY, width, height, color, font)
    width = width or 18
    height = height or 18
    font = font or CMAKE_SOURCE_DIR .. "res/MsYahei.ttf"
    color = color or 0x00ff0000
    local unicodes = {}
    local i = 1
    while i <= #msg do
        local code = string.byte(msg, i, i)

        if (code & 0x80) == 0 then
            unicodes[#unicodes + 1] = code
            i = i + 1
        else
            local to = i
            code = (code << 1) & 0xff -- code 就是第一个
            while code > 0x7f do
                code = (code << 1) & 0xff
                to = to + 1
            end
            local unicode = code >> (to - i + 1)
            for codeIdx = i + 1, to do
                unicode = unicode << 6
                local subCode = string.byte(msg, codeIdx, codeIdx) & 0x3f
                unicode = unicode + subCode
            end
            i = to + 1
            unicodes[#unicodes + 1] = unicode
        end
    end
    local bmps = {}
    for _, unicode in ipairs(unicodes) do
        local bmp = GetFontBmp(unicode, width, height, color, font)
        bmps[#bmps + 1] = bmp
    end
    local offset = 0
    for _, bmp in ipairs(bmps) do
        Framework:draw(bmp, atX + offset, atY, false)
        offset = offset + #bmp[1]
    end
end
