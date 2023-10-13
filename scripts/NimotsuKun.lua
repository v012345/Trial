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
                    if worker_pos.x == x and worker_pos.y == y then
                        if cell == 5 then
                            io.write(OBJ[2])
                        else
                            io.write(OBJ[1])
                        end
                    else
                        local has_horse = false
                        for _, horse in ipairs(HORSES) do
                            if horse.x == x and horse.y == y then
                                if cell == 5 then
                                    io.write(OBJ[4])
                                else
                                    io.write(OBJ[3])
                                end
                                has_horse = true
                                break
                            end
                        end
                        if not has_horse then
                            io.write(OBJ[cell])
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
            end
            if input == "q" then
                break
            end
        end
        stageId = coroutine.yield(stageId)
    end
end)
local _, stageId = coroutine.resume(SelectStageThread, 1)
while true do
    _, stageId = coroutine.resume(PlayGameThread, stageId)
    _, stageId = coroutine.resume(SelectStageThread, stageId)
end
