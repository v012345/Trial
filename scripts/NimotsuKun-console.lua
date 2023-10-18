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
                local dst = Impl:colorAt(atX + i - 1, atY + y)
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
    local PlayGameThread = coroutine.create(function(stageId)
        while true do
            os.execute("cls")
            Impl:clear()
            print("Stage " .. stageId)
            local MAN = '😁' -- 1
            local MAN_ON_GRASS = '😬' -- 2
            local HORSE = '🐴' -- 3
            local UNICORN = '🦄' -- 4
            local GRASS = '🍀' -- 5
            local LOG = '🪵' -- 6
            local OBJ = {
                MAN, MAN_ON_GRASS, HORSE, UNICORN, GRASS, LOG, [0] = "  "
            }
            print("asdw 移动, r 重置本关, q 退出")
            local factory = {}
            for x, row in ipairs((require "Stages")[stageId]) do
                factory[x] = factory[x] or {}
                for y, obj in ipairs(row) do
                    factory[x][y] = obj
                end
            end
            local factory_backup = {}
            for x, row in ipairs(factory) do
                factory_backup[x] = factory_backup[x] or {}
                for y, obj in ipairs(row) do
                    factory_backup[x][y] = obj
                end
            end

            local function parse_factory(factory_map)
                local worker_pos = nil
                local HORSES = {}
                for x, row in ipairs(factory_map) do
                    for y, obj in ipairs(row) do
                        if obj == 1 then
                            if worker_pos then
                                error("has more one worker")
                            else
                                worker_pos = { x = x, y = y }
                                factory_map[x][y] = 0
                            end
                        end
                        if obj == 2 then
                            if worker_pos then
                                error("has more one worker")
                            else
                                worker_pos = { x = x, y = y }
                                factory_map[x][y] = 5
                            end
                        end
                        if obj == 3 then
                            HORSES[#HORSES + 1] = { x = x, y = y }
                            factory_map[x][y] = 0
                        end
                        if obj == 4 then
                            HORSES[#HORSES + 1] = { x = x, y = y }
                            factory_map[x][y] = 5
                        end
                    end
                end
                if worker_pos and #HORSES > 0 then
                    return worker_pos, HORSES
                else
                    error("wrong factory map")
                end
            end
            local worker_pos, HORSES = parse_factory(factory)

            local function reset_game()
                for x, row in ipairs(factory_backup) do
                    factory[x] = factory[x] or {}
                    for y, obj in ipairs(row) do
                        factory[x][y] = obj
                    end
                end
                worker_pos, HORSES = parse_factory(factory)
            end

            -- 渲染地图
            local function draw()
                for x, row in ipairs(factory) do
                    for y, cell in ipairs(row) do
                        drawCell(nimotsuKunImage, 5, x * 32 - 32, y * 32 - 32)
                        if worker_pos.x == x and worker_pos.y == y then
                            if cell == 5 then
                                io.write(OBJ[2])
                                drawCell(nimotsuKunImage, 4, x * 32 - 32, y * 32 - 32)
                            else
                                io.write(OBJ[1])
                            end
                            drawCell(nimotsuKunImage, 1, x * 32 - 32, y * 32 - 32)
                        else
                            local has_horse = false
                            for _, horse in ipairs(HORSES) do
                                if horse.x == x and horse.y == y then
                                    if cell == 5 then
                                        io.write(OBJ[4])
                                        drawCell(nimotsuKunImage, 4, x * 32 - 32, y * 32 - 32)
                                    else
                                        io.write(OBJ[3])
                                    end
                                    drawCell(nimotsuKunImage, 3, x * 32 - 32, y * 32 - 32)
                                    has_horse = true
                                    break
                                end
                            end
                            if not has_horse then
                                io.write(OBJ[cell])
                                -- if cell == 0 then
                                --     drawCell(nimotsuKunImage, 5, x * 32, y * 32)
                                -- end
                                if cell == 1 then
                                    drawCell(nimotsuKunImage, 1, x * 32 - 32, y * 32 - 32)
                                end
                                if cell == 2 then
                                    drawCell(nimotsuKunImage, 1, x * 32 - 32, y * 32 - 32)
                                end
                                if cell == 3 then
                                    drawCell(nimotsuKunImage, 3, x * 32 - 32, y * 32 - 32)
                                end
                                if cell == 4 then
                                    drawCell(nimotsuKunImage, 3, x * 32 - 32, y * 32 - 32)
                                end
                                if cell == 5 then
                                    drawCell(nimotsuKunImage, 4, x * 32 - 32, y * 32 - 32)
                                end
                                if cell == 6 then
                                    drawCell(nimotsuKunImage, 2, x * 32 - 32, y * 32 - 32)
                                end
                            end
                        end
                    end
                    io.write("\n")
                end
            end
            local function is_horse(x, y)
                for _, horse in ipairs(HORSES) do
                    if horse.x == x and horse.y == y then
                        return true, horse
                    end
                end
                return false, {}
            end
            local function can_stand(x, y)
                if factory[x][y] == 6 then
                    return false
                end
                return true
            end
            -- 通关
            local function check_clear()
                for _, horse in ipairs(HORSES) do
                    if factory[horse.x][horse.y] ~= 5 then
                        return false
                    end
                end
                return true
            end
            local function deal_input(input)
                local old_x = worker_pos.x
                local old_y = worker_pos.y
                local dx = 0
                local dy = 0
                if input == "a" then
                    dy = -1
                elseif input == "s" then
                    dx = 1
                elseif input == "d" then
                    dy = 1
                elseif input == "w" then
                    dx = -1
                elseif input == "r" then
                    reset_game()
                    return
                end
                local isHorse, horse = is_horse(old_x + dx, old_y + dy)
                if isHorse then
                    if can_stand(horse.x + dx, horse.y + dy) then
                        if not is_horse(horse.x + dx, horse.y + dy) then
                            worker_pos.x = old_x + dx
                            worker_pos.y = old_y + dy
                            horse.x = horse.x + dx
                            horse.y = horse.y + dy
                        end
                    end
                elseif can_stand(old_x + dx, old_y + dy) then
                    worker_pos.x = old_x + dx
                    worker_pos.y = old_y + dy
                end
            end
            draw()
            while true do
                if check_clear() then
                    print("Congratulation!(Press Any key to next stage)")
                    _getch()
                    stageId = stageId + 1
                    break
                end
                local input = string.char(_getch())
                local old_x = worker_pos.x
                local old_y = worker_pos.y
                deal_input(input)
                if not (old_x == worker_pos.x and old_y == worker_pos.y) or input == "r" then
                    SetConsoleCursorPosition(0, 2)
                    draw()
                    coroutine.yield(stageId, true)
                end
                if input == "q" then
                    break
                end
            end
            stageId = coroutine.yield(stageId, false)
        end
    end)
    local stageId = 1
    local PlayGameThreadLoop = false
    function MainLoop()
        -- for i = 1, 5, 1 do
        -- drawCell(nimotsuKunImage, i, i * 32, i * 32)
        -- end
        -- drawImage(nimotsuKunImage, 0, 0)
        -- drawCell(nimotsuKunImage, 1, 1 * 32, 1 * 32)
        if not PlayGameThreadLoop then
            _, stageId = coroutine.resume(SelectStageThread, stageId)
        end
        _, stageId, PlayGameThreadLoop = coroutine.resume(PlayGameThread, stageId)
        -- print(_getch)
        -- print(_getch())
    end
end, function(msg)
    print(msg)
end)
