Entity = {}
---@class Entity
local mt = {}
function Entity:new(EntityConfig)
    ---@class Entity
    local object = {
        Sprite = EntityConfig.Sprite
    }
    setmetatable(object, { __index = mt })
    return object
end

function mt:update()

end
