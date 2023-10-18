local Stages = {
    [[
        🪵🪵🪵🪵🪵🪵🪵
        🪵😁⬛🐴🍀⬛🪵
        🪵⬛⬛🐴⬛⬛🪵
        🪵🍀⬛⬛⬛⬛🪵
        🪵🪵🪵🪵🪵🪵🪵
    ]],
    [[
    ⬛⬛🪵🪵🪵⬛⬛⬛
    ⬛⬛🪵🍀🪵⬛⬛⬛
    ⬛⬛🪵⬛🪵🪵🪵🪵
    🪵🪵🪵🐴⬛🐴🍀🪵
    🪵🍀⬛🐴😁🪵🪵🪵
    🪵🪵🪵🪵🐴🪵⬛⬛
    ⬛⬛⬛🪵🍀🪵⬛⬛
    ⬛⬛⬛🪵🪵🪵⬛⬛
    ]],
    [[
    ⬛⬛🪵🪵🪵🪵⬛⬛
    ⬛⬛🪵🍀🍀🪵⬛⬛
    ⬛🪵🪵⬛🍀🪵🪵⬛
    ⬛🪵⬛⬛🐴🍀🪵⬛
    🪵🪵⬛🐴⬛⬛🪵🪵
    🪵⬛⬛🪵🐴🐴⬛🪵
    🪵⬛⬛😁⬛⬛⬛🪵
    🪵🪵🪵🪵🪵🪵🪵🪵
    ]],

}

local function get_char_lenght(head)
    if ((head & 0xc0) ~ 0x80) == 0 then
        error("not a utf-8 head")
    end
    if (head & 0x80) == 0 then
        return 1
    elseif (head & 0x20) == 0 then
        return 2
    elseif (head & 0x10) == 0 then
        return 3
    elseif (head & 0x08) == 0 then
        return 4
    elseif (head & 0x04) == 0 then
        return 5
    elseif (head & 0x02) == 0 then
        return 6
    elseif (head & 0x01) == 0 then
        return 7
    end
end

local function get_a_char(content, position)
    local head = string.byte(content, position, position)
    if (head & 0x80) ~= 0 then
        local length = get_char_lenght(head) - 1
        local char = string.sub(content, position, position + length)
        position = position + length + 1
        return char, position
    else
        return string.char(head), position + 1
    end
end

local res = {}
for _, raw_stage in ipairs(Stages) do
    local char, position = get_a_char(raw_stage, 1)
    local map = { {} }
    while position < #raw_stage do
        char, position = get_a_char(raw_stage, position)
        if char ~= " " then
            if char == "⬛" then
                table.insert(map[#map], 0)
            elseif char == "😁" then
                table.insert(map[#map], 1)
            elseif char == "😬" then
                table.insert(map[#map], 2)
            elseif char == "🐴" then
                table.insert(map[#map], 3)
            elseif char == "🦄" then
                table.insert(map[#map], 4)
            elseif char == "🍀" then
                table.insert(map[#map], 5)
            elseif char == "🪵" then
                table.insert(map[#map], 6)
            elseif char == "\n" then
                map[#map + 1] = {}
            end
        end
    end
    map[#map] = nil
    res[#res + 1] = map
end
return res
