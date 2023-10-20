---comment
---@param data Entity
---@return Entity
function CreateEntity(data)
    ---@class Entity
    Entity = {
        x = 0,
        y = 0,
        screenX = 0,
        screenY = 0,
        toScreenX = 0,
        toScreenY = 0,
        eRenderEntity = 0,
        isBackground = false,
        counter = 0
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
        Framework:draw(RenderEntity[self.eRenderEntity], self.screenX, self.screenY, self.isBackground)
    end

    function Entity:draw()
        Framework:draw(RenderEntity[self.eRenderEntity], self.screenX, self.screenY, self.isBackground)
    end

    Entity.screenX = data.screenX
    Entity.screenY = data.screenY
    Entity.eRenderEntity = data.eRenderEntity
    Entity.x = data.x or 0
    Entity.y = data.y or 0
    Entity.isBackground = data.isBackground or false

    return Entity
end
