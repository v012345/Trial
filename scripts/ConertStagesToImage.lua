local NimotsuKunImags = ReadPngFile(CMAKE_SOURCE_DIR .. "res/nimotsuKunImageAlpha.png")
---@param IMAGE table<table<integer>>
local function cutImage(IMAGE)
    local r = {}

    for y, row in ipairs(IMAGE) do
        for i = 1, 5, 1 do
            r[i] = r[i] or {}
            r[i][y] = r[i][y] or {}
            local t = r[i][y]
            for j = 1, 32, 1 do
                t[#t + 1] = row[32 * (i - 1) + j]
            end
        end
    end
    return r
end

local r = cutImage(NimotsuKunImags)
local entity = {}
entity.worker = r[1]
entity.wall = r[2]
entity.floor = r[5]
entity.goal = r[4]
entity.box = r[3]

return entity
