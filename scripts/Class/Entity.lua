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
    if Framework.isKeyOn(Keyboard.W) then
        self.Sprite:setDiretion(Direction.Up)
    end
    if Framework.isKeyOn(Keyboard.S) then
        self.Sprite:setDiretion(Direction.Down)
    end
    if Framework.isKeyOn(Keyboard.A) then
        self.Sprite:setDiretion(Direction.Left)
    end
    if Framework.isKeyOn(Keyboard.D) then
        self.Sprite:setDiretion(Direction.Right)
    end
    if Framework.isKeyOn(Keyboard.Shift) then
        self.Sprite:setAction("run")
    else
        self.Sprite:setAction("walk")
    end
    Framework:draw(self.Sprite:getNextFrame(), 0, 0, false)
end
