xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    local nimotsuKunImage = ReadPngFile(CMAKE_SOURCE_DIR .. "/res/translucent.png")
    -- for i = 1, nimotsuKunImage.height, 1 do
    --     for j = 1, nimotsuKunImage.width, 1 do
    --         local argb = nimotsuKunImage.ARGB[(i - 1) * nimotsuKunImage.width + j]
    --         local srcA = (argb & 0xff000000) >> 24
    --         local srcR = (argb & 0xff0000) >> 16;
    --         local srcG = (argb & 0x00ff00) >> 8;
    --         local srcB = argb & 0x0000ff;
    --         print(argb)
    --         print(srcA, srcR, srcG, srcB)
    --     end
    -- end
    ---@param IMAGE image
    ---@param TYPE integer
    local function drawCell(IMAGE, TYPE, atX, atY)
        for i = 1, IMAGE.height, 1 do
            local y = 0
            for j = (TYPE - 1) * 32 + 1, TYPE * 32, 1 do
                Impl:vram(atX + i - 1, atY + y, IMAGE.ARGB[i * IMAGE.width + j])
                y = y + 1
            end
        end
    end

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
    function MainLoop()
        -- for i = 1, 5, 1 do
        -- drawCell(nimotsuKunImage, i, i * 32, i * 32)
        -- end
        drawImage(nimotsuKunImage, 0, 0)
    end
end, function(msg)
    print(msg)
end)
