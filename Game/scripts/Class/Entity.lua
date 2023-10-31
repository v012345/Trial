Entity = {}
---@class Entity
local mt = {}
function Entity:new(EntityConfig)
    ---@class Entity
    ---@field Sprite Sprite
    local object = {
        Sprite = EntityConfig.Sprite
    }
    setmetatable(object, { __index = mt })
    return object
end

function mt:update()
    self:dealInput()
    Framework:draw(self.Sprite:getNextFrame(), 0, 0, false)
end

function mt:dealInput()
    error("must implementions function dealInput")
end
