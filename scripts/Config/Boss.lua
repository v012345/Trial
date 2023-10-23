---@type SpriteConfig
local LinkSprite = {
    path = CMAKE_SOURCE_DIR .. "res/professor_walk_cycle_no_hat.png",
    cell_height = 64,
    cell_width = 64,
    [Direction.Up] = {
        idle = {
            frame = 9,
            row = 1,
            sequence = {}
        },
        walk = {
            frame = 9,
            row = 1,
            sequence = {}
        }
    },
    [Direction.Down] = {
        idle = {
            frame = 9,
            row = 3,
            sequence = {}
        },
        walk = {
            frame = 9,
            row = 3,
            sequence = {}
        }
    },
    [Direction.Left] = {
        idle = {
            frame = 9,
            row = 2,
            sequence = {}
        },
        walk = {
            frame = 9,
            row = 2,
            sequence = {}
        }
    },
    [Direction.Right] = {
        idle = {
            frame = 9,
            row = 4,
            sequence = {}
        },
        walk = {
            frame = 9,
            row = 4,
            sequence = {}
        }
    }
}
return LinkSprite
