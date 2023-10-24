---@class SpriteConfig
local BlueMan = {
    image = CMAKE_SOURCE_DIR .. "res/8D MZ RTP/8Directionals/8D_0005_actor2-1[VS8].png",
    cell_height = 100,
    cell_width = 100,
    interval = 8,
    actions = { "walk", "run", "idle" },
    [Direction.Up] = {
        idle = { 38 },
        walk = { 37, 38, 39, 38 },
        run = { 43, 44, 45, 44 },
    },
    [Direction.Down] = {
        idle = { 2 },
        walk = { 1, 2, 3, 2 },
        run = { 7, 8, 9, 8 },
    },
    [Direction.Left] = {
        idle = { 14 },
        walk = { 13, 14, 15, 14 },
        run = { 19, 20, 21, 20 },
    },
    [Direction.Right] = {
        idle = { 26 },
        walk = { 25, 26, 27, 26 },
        run = { 31, 32, 33, 32 },
    },
    [Direction.RightDown] = {
        idle = { 17 },
        walk = { 16, 17, 18, 17 },
        run = { 22, 23, 24, 23 },
    },
    [Direction.RightUp] = {
        idle = { 41 },
        walk = { 40, 41, 42, 41 },
        run = { 46, 47, 48, 47 },
    },
    [Direction.LeftDown] = {
        idle = { 5 },
        walk = { 4, 5, 6, 5 },
        run = { 10, 11, 12, 11 },
    },
    [Direction.LeftUp] = {
        idle = { 29 },
        walk = { 28, 29, 30, 29 },
        run = { 34, 35, 36, 35 },
    },
    sequence = {
        [Direction.Up] = {
            idle = {},
            walk = {},
            run  = {},
        },
        [Direction.Down] = {
            idle = {},
            walk = {},
            run  = {},
        },
        [Direction.Left] = {
            idle = {},
            walk = {},
            run  = {},
        },
        [Direction.Right] = {
            idle = {},
            walk = {},
            run  = {},
        },
        [Direction.RightUp] = {
            idle = {},
            walk = {},
            run  = {},
        },
        [Direction.RightDown] = {
            idle = {},
            walk = {},
            run  = {},
        },
        [Direction.LeftUp] = {
            idle = {},
            walk = {},
            run  = {},
        },
        [Direction.LeftDown] = {
            idle = {},
            walk = {},
            run  = {},
        },
    }
}

local rawImage = ReadPngFile(BlueMan.image)

---@param PNG Image
---@param fromX integer 包括
---@param fromY integer 包括
---@param toX integer 不包括
---@param toY integer 不包括
---@return Image
local function getBlock(PNG, fromX, fromY, toX, toY)
    ---@type Image
    local piece = {}
    for y = fromY, toY - 1 do
        local row = {}
        for x = fromX, toX - 1 do
            row[#row + 1] = PNG[y][x]
        end
        piece[#piece + 1] = row
    end
    return piece
end

---@param SpriteConfig SpriteConfig
local function convertIdxToBlock(rawImage, SpriteConfig, idx)
    idx = idx - 1
    local width = SpriteConfig.cell_width
    local height = SpriteConfig.cell_height
    local col_number = #rawImage[1] // width
    local row_idx = idx // col_number
    local col_idx = idx % col_number
    local fromX = col_idx * width + 1
    local fromY = row_idx * height + 1
    local toX = col_idx * width + width + 1
    local toY = row_idx * height + height + 1
    return getBlock(rawImage, fromX, fromY, toX, toY)
end

for _, direction in pairs(Direction) do
    if direction ~= Direction.Null then
        for _, action in ipairs(BlueMan.actions) do
            for _, idx in ipairs(BlueMan[direction][action]) do
                table.insert(BlueMan.sequence[direction][action],
                    convertIdxToBlock(rawImage, BlueMan, idx))
            end
        end
    end
end


return BlueMan
