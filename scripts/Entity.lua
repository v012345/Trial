---comment
---@param data Entity
---@return Entity
function CreateEntity(data)
    ---@class Entity
    Entity = {
        x = 0,
        y = 0,
        dx = 0,
        dy = 0,
        screenX = 0,
        screenY = 0,
        toScreenX = 0,
        toScreenY = 0,
        eRenderEntity = 0,
        isBackground = false,
        counter = 0,
    }
    function Entity:move(dx, dy)
        if dx ~= 0 or dy ~= 0 then
            if self.screenX == self.toScreenX and self.screenY == self.toScreenY then
                self.x = self.x + dx
                self.y = self.y + dy
                self.dx = dx
                self.dy = dy
                self.toScreenX = self.screenX + dx * Game.SpriteSize
                self.toScreenY = self.screenY + dy * Game.SpriteSize
            end
        end
    end

    function Entity:update(dt)
        if not (self.screenX == self.toScreenX and self.screenY == self.toScreenY) then
            self.screenX = self.screenX + self.dx
            self.screenY = self.screenY + self.dy
        else
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
    Entity.toScreenX = data.screenX
    Entity.toScreenY = data.screenY
    Entity.eRenderEntity = data.eRenderEntity
    Entity.x = data.x or 0
    Entity.y = data.y or 0
    Entity.isBackground = data.isBackground or false

    return Entity
end
