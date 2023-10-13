local MAN = '😁' -- 1
local MAN_ON_GRASS = '😬' -- 2
local HORSE = '🐴' -- 3
local UNICORN = '🦄' -- 4
local GRASS = '🍀' -- 5
local LOG = '🪵' -- 6
local OBJ = {
    MAN, MAN_ON_GRASS, HORSE, UNICORN, GRASS, LOG, [0] = "  "
}
print("a,s,d,w to move")
local factory = {
    { 0, 0, 6, 6, 6, 6, 0, 0 },
    { 0, 0, 6, 5, 5, 6, 0, 0 },
    { 0, 6, 0, 0, 5, 6, 6, 0 },
    { 0, 6, 0, 0, 3, 5, 6, 0 },
    { 6, 6, 0, 3, 0, 0, 6, 6 },
    { 6, 0, 0, 6, 3, 3, 0, 6 },
    { 6, 0, 0, 1, 0, 0, 0, 6 },
    { 6, 6, 6, 6, 6, 6, 6, 6 },
}
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
        print("Congratulation!")
        break
    end
    local input = string.char(_getch())
    local old_x = worker_pos.x
    local old_y = worker_pos.y
    deal_input(input)
    if not (old_x == worker_pos.x and old_y == worker_pos.y) then
        SetConsoleCursorPosition(0, 2)
        draw()
    end
end
os.execute("pause")
