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
    Framework:draw(self.Sprite:getNextFrame(), 0, 0, true)
end
