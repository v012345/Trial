xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    local nimotsuKunImage = ReadPngFile(CMAKE_SOURCE_DIR .. "/res/nimotsuKunImageAlpha.png")
    ---@param IMAGE image
    ---@param TYPE integer
    local function drawCell(IMAGE, TYPE, atX, atY)
        for i = 1, IMAGE.height, 1 do
            local y = 0
            for j = (TYPE - 1) * 32 + 1, TYPE * 32, 1 do
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
                -- Impl:vram(atX + i - 1, atY + j - 1, dst)
                Impl:vram(atX + i - 1, atY + y, dst)
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
    os.execute("cls")
    local SelectStageThread = coroutine.create(function(stageId)
        local currentStage = stageId
        local allStages = #(require "Stages")
        local from = (currentStage - 1) // 10 * 10 + 1
        local to = from + 9
        while true do
            print("asdw 选择, 回车确认, q退出")
            if to > allStages then
                to = allStages
            end
            for i = from, to, 1 do
                if currentStage == i then
                    print(" ➡️ Stage " .. i)
                else
                    print("    Stage " .. i)
                end
            end
            print("👋")
            local input_code = _getch()
           
            local input = string.char(input_code)
            if input == "q" then
                os.exit(0, true)
            elseif input == "s" then
                currentStage = currentStage + 1
            elseif input == "w" then
                currentStage = currentStage - 1
            elseif input == "a" then
                currentStage = currentStage - 10
            elseif input == "d" then
                currentStage = currentStage + 10
            end
            if input_code == 13 then
                os.execute("cls")
                currentStage = coroutine.yield(currentStage)
                os.execute("cls")
            end
            if currentStage < 1 then
                currentStage = 1
            end
            if currentStage > allStages then
                currentStage = allStages
            end
            SetConsoleCursorPosition(0, 0)
            if currentStage < from or currentStage > to then
                local new_from = (currentStage - 1) // 10 * 10 + 1
                for i = from, to, 1 do
                    if i - from + new_from > allStages then
                        print("                    ")
                    else
                        print("    Stage           ")
                    end
                end
                from = (currentStage - 1) // 10 * 10 + 1
                to = from + 9
                SetConsoleCursorPosition(0, 0)
            end
        end
    end)
    local _, stageId = coroutine.resume(SelectStageThread, 1)
    function MainLoop()
        -- for i = 1, 5, 1 do
        -- drawCell(nimotsuKunImage, i, i * 32, i * 32)
        -- end
        -- drawImage(nimotsuKunImage, 0, 0)
        -- _, stageId = coroutine.resume(PlayGameThread, stageId)
        _, stageId = coroutine.resume(SelectStageThread, stageId)
        -- print(_getch)
        -- print(_getch())
    end
end, function(msg)
    print(msg)
end)
