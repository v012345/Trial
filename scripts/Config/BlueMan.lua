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

return BlueMan

