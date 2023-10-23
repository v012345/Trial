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
    local rawImage = ReadPngFile(SpriteConfig.image)
    setmetatable(object, { __index = mt })
    return object
end

function mt:setDiretion(direction)
    self.mDirection = direction
end

function mt:getDiretion()
    return self.mDirection
end
