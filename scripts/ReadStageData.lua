local file = io.open(CMAKE_SOURCE_DIR .. "example/stageData.txt", "r") or
    error("can't open " .. CMAKE_SOURCE_DIR .. "example/stageData.txt")
local rawStageData = file:read()
local map = {}
while rawStageData do
    local row = {}
    for i = 1, #rawStageData do
        local char = string.sub(rawStageData, i, i)
        if char == " " then
            row[#row + 1] = ENUM.ground
        elseif char == "#" then
            row[#row + 1] = ENUM.wall
        elseif char == "p" then
            row[#row + 1] = ENUM.player
        elseif char == "." then
            row[#row + 1] = ENUM.goal
        elseif char == "o" then
            row[#row + 1] = ENUM.box
        else
            error(string.format("wrong char '%s' at line %s:%s", char, #map + 1, i))
        end
    end
    map[#map + 1] = row
    rawStageData = file:read()
end
file:close()

return map
