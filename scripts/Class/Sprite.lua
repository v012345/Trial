Sprite = {}
---@class Sprite
local mt = {}
---comment
---@param SpriteConfig SpriteConfig
---@return Sprite
function Sprite:new(SpriteConfig)
    ---@class Sprite
    local object = {
        mDirection = Direction.Down,
        SpriteConfig = SpriteConfig,
        mAction = "walk",
        mFrame = 1
    }
    setmetatable(object, { __index = mt })
    return object
end

function mt:getNextFrame()
    local action = self.SpriteConfig.sequence[self.mDirection][self.mAction]
    if self.mFrame > #action then
        self.mFrame = 1
    end
    local frame = action[self.mFrame]
    self.mFrame = self.mFrame + 1
    return frame
end

function mt:setDiretion(direction)
    self.mDirection = direction
end

function mt:getDiretion()
    return self.mDirection
end
