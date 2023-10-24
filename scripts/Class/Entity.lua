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
    local direction = Direction.Null
    if Framework.isKeyOn(Keyboard.W) then
        direction = direction | Input[Keyboard.W]
    end
    if Framework.isKeyOn(Keyboard.S) then
        direction = direction | Input[Keyboard.S]
    end
    if Framework.isKeyOn(Keyboard.A) then
        direction = direction | Input[Keyboard.A]
    end
    if Framework.isKeyOn(Keyboard.D) then
        direction = direction | Input[Keyboard.D]
    end
    self.Sprite:setDiretion(InputToDirection[direction])
    if Framework.isKeyOn(Keyboard.Shift) then
        self.Sprite:setAction("run")
    else
        self.Sprite:setAction("walk")
    end
    Framework:draw(self.Sprite:getNextFrame(), 0, 0, false)
end
