xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    local nimotsuKunImage = ReadPngFile(CMAKE_SOURCE_DIR .. "/res/transparent.png")
    ---@param IMAGE image
    ---@param TYPE integer
    local function drawCell(IMAGE, TYPE, atX, atY)
        for i = 1, IMAGE.height, 1 do
            local y = 0
            for j = (TYPE - 1) * 32 + 1, TYPE * 32, 1 do
                Impl:vram(atX + i - 1, atY + y, IMAGE.RGB[i * IMAGE.width + j])
                y = y + 1
            end
        end
    end

    ---@param IMAGE image
    local function drawImage(IMAGE, atX, atY)
        for i = 1, IMAGE.height, 1 do
            for j = 1, IMAGE.width, 1 do
                Impl:vram(atX + i - 1, atY + j - 1, IMAGE.RGB[i * IMAGE.width + j])
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
