---@class SpriteConfig
local BlueMan = {
    image = CMAKE_SOURCE_DIR .. "res/8D MZ RTP/8Directionals/8D_0005_actor2-1[VS8].png",
    cell_height = 100,
    cell_width = 100,
    actions = { "walk", "run" },
    [Direction.Up] = {
        walk = { 1, 2, 3 },
        run = { 4, 5, 6 },
    },
    [Direction.Down] = {
        walk = { 7, 8, 9 },
        run = { 10, 11, 12 },
    },
    [Direction.Left] = {
        walk = { 13, 14, 15 },
        run = { 16, 17, 18 },
    },
    [Direction.Right] = {
        walk = { 19, 20, 21 },
        run = { 22, 23, 24 },
    },
    [Direction.RightDown] = {
        walk = { 25, 26, 27 },
        run = { 28, 29, 30 },
    },
    [Direction.RightUp] = {
        walk = { 31, 32, 33 },
        run = { 34, 35, 36 },
    },
    [Direction.LeftDown] = {
        walk = { 37, 38, 39 },
        run = { 40, 41, 42 },
    },
    [Direction.LeftUp] = {
        walk = { 43, 44, 45 },
        run = { 46, 47, 48 },
    },
    sequence = {
        [Direction.Up] = {
            walk = {},
            run = {},
        },
        [Direction.Down] = {
            walk = {},
            run = {},
        },
        [Direction.Left] = {
            walk = {},
            run = {},
        },
        [Direction.Right] = {
            walk = {},
            run = {},
        },
        [Direction.RightUp] = {
            walk = {},
            run = {},
        },
        [Direction.RightDown] = {
            walk = {},
            run = {},
        },
        [Direction.LeftUp] = {
            walk = {},
            run = {},
        },
        [Direction.LeftDown] = {
            walk = {},
            run = {},
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
    for _, action in ipairs(BlueMan.actions) do
        for _, idx in ipairs(BlueMan[direction][action]) do
            table.insert(BlueMan.sequence[direction][action],
                convertIdxToBlock(rawImage, BlueMan, idx))
        end
    end
end


return BlueMan
