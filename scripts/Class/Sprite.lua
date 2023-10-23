Sprite = {}
---@class Sprite
local mt = {}
---comment
---@param SpriteConfig SpriteConfig
---@return Sprite
function Sprite:new(SpriteConfig)
    ---@class Sprite
    local object = {
        mDirection = Direction.Down
    }
    setmetatable(object, { __index = mt })
    local rawImage = ReadPngFile(SpriteConfig.image)
    return object
end

function mt:setDiretion(direction)
    self.mDirection = direction
end

function mt:getDiretion()
    return self.mDirection
end

function mt:convertIdxToXY()
    
end

---@param PNG Image
---@param fromX integer 包括
---@param fromY integer 包括
---@param toX integer 不包括
---@param toY integer 不包括
---@return Image
function mt:getBlock(PNG, fromX, fromY, toX, toY)
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
