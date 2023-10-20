local function CreateEntity(data)
    ---@class Entity
    Entity = {
        screenX = 0,
        screenY = 0,
        toScreenX = 0,
        toScreenY = 0,
    }
    function Entity:move(dx, dy)
        if self.counter <= 0 then
            self.dx = dx
            self.dy = dy
            self.counter = Game.SpriteSize
        end
    end

    function Entity:update(dt)
        if self.counter >= 1 then
            self.screenX = self.screenX + self.dx
            self.screenY = self.screenY + self.dy
            self.counter = self.counter - 1
        else
            self.counter = 0
            self.dx = 0
            self.dy = 0
        end
        Framework:draw(RenderEntity[Enum.player], self.screenX, self.screenY, false)
    end

    return Entity
end


return CreateEntity
