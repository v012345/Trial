---@class Res_Sprite
local LinkSprite = {
    path = CMAKE_SOURCE_DIR .. "res/Link.png",
    cell_height = 104,
    cell_width = 96,
    [Direction.Up] = {
        idle = {
            frame = 1,
            row = 3,
            sequence = {}
        },
        walk = {
            frame = 10,
            row = 7,
            sequence = {}
        }
    },
    [Direction.Down] = {
        idle = {
            frame = 3,
            row = 1,
            sequence = {}
        },
        walk = {
            frame = 10,
            row = 5,
            sequence = {}
        }
    },
    [Direction.Left] = {
        idle = {
            frame = 3,
            row = 2,
            sequence = {}
        },
        walk = {
            frame = 10,
            row = 5,
            sequence = {}
        }
    },
    [Direction.Right] = {
        idle = {
            frame = 3,
            row = 4,
            sequence = {}
        },
        walk = {
            frame = 10,
            row = 8,
            sequence = {}
        }
    }
}
return LinkSprite
