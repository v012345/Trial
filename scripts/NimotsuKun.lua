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
local Ground = {
    { 6, 6, 6, 6, 6, 6, 6, 6 },
    { 6, 0, 5, 5, 0, 0, 0, 6 },
    { 6, 0, 0, 0, 0, 0, 0, 6 },
    { 6, 0, 0, 0, 0, 0, 0, 6 },
    { 6, 6, 6, 6, 6, 6, 6, 6 },
}
local position = { x = 4, y = 2 }
local HORSES = {
    { x = 3, y = 3 },
    { x = 3, y = 4 }
}

local function draw()
    for x, row in ipairs(Ground) do
        for y, cell in ipairs(row) do
            if position.x == x and position.y == y then
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
local function is_clear()
    for _, horse in ipairs(HORSES) do
        if Ground[horse.x][horse.y] ~= 5 then
            return false
        end
    end
    return true
end
local function deal_input(input)
    local old_x = position.x
    local old_y = position.y
    if input == "a" then
        position.y = position.y - 1
    elseif input == "s" then
        position.x = position.x + 1
    elseif input == "d" then
        position.y = position.y + 1
    elseif input == "w" then
        position.x = position.x - 1
    end
    for _, horse in ipairs(HORSES) do
        if horse.x == position.x and horse.y == position.y then
            local h_old_x = horse.x
            local h_old_y = horse.y
            if input == "a" then
                horse.y = horse.y - 1
            elseif input == "s" then
                horse.x = horse.x + 1
            elseif input == "d" then
                horse.y = horse.y + 1
            elseif input == "w" then
                horse.x = horse.x - 1
            end
            if horse.x < 2 or horse.x > #Ground - 1 then
                horse.x = h_old_x
                position.x = old_x
                return
            end
            if horse.y < 2 or horse.y > #Ground[1] - 1 then
                horse.y = h_old_y
                position.y = old_y
                return
            end
            for _, other_horse in ipairs(HORSES) do
                if other_horse ~= horse then
                    if horse.x == other_horse.x and horse.y == other_horse.y then
                        horse.y = h_old_y
                        position.y = old_y
                        return
                    end
                end
            end
            break
        end
    end
    if position.x < 2 or position.x > #Ground - 1 then
        position.x = old_x
    end
    if position.y < 2 or position.y > #Ground[1] - 1 then
        position.y = old_y
    end
end
draw()
while true do
    local input = string.char(_getch())
    local old_x = position.x
    local old_y = position.y
    deal_input(input)
    if not (old_x == position.x and old_y == position.y) then
        SetConsoleCursorPosition(0, 2)
        draw()
        if is_clear() then
            print("Congratulation!")
            break
        end
    end
end
os.execute("pause")
