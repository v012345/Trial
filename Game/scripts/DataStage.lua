local lfs = require "lfs"
require "Enum"
DataStage = {
    ---@type table<StageData>
    stage = {}
}
function DataStage:init()
    local folder = CMAKE_CURRENT_SOURCE_DIR .. "data/"
    local stageData = {}
    for entry in lfs.dir(folder) do
        if entry ~= "." and entry ~= ".." then
            local filePath = folder .. entry
            local fileAttributes = lfs.attributes(filePath)
            if fileAttributes.mode == "file" then
                if string.match(string.lower(filePath), "^.+%.txt$") then
                    local stageId = tonumber((string.gsub(entry, "%.txt", "", 1)))
                    if stageId then
                        stageData[stageId] = filePath
                    end
                end
            end
        end
    end
    for _, path in ipairs(stageData) do
        self.stage[#self.stage + 1] = self:parserStage(path)
    end
end

function DataStage:parserStage(path)
    local background = {}
    local goals = {}
    local boxes = {}
    local worker = {}
    local file = io.open(path, "r") or error()
    local line = file:read("l")
    local line_number = 1
    while line do
        local row = {}
        for i = 1, #line do
            local char = string.sub(line, i, i)
            if char == "#" then
                row[#row + 1] = Enum.Obj.wall
            else
                row[#row + 1] = Enum.Obj.ground
            end
            if char == "p" then
                worker.y = line_number
                worker.x = i
            elseif char == "." then
                goals[#goals + 1] = {
                    x = i,
                    y = line_number
                }
            elseif char == "o" then
                boxes[#boxes + 1] = {
                    x = i,
                    y = line_number
                }
            end
        end
        background[#background + 1] = row
        line_number = line_number + 1
        line = file:read("l")
    end
    ---@class StageData
    local r = {
        background = background,
        player = worker,
        goals = goals,
        boxes = boxes,
    }
    return r
end

DataStage:init()
