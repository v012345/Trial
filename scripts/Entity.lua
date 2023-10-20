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
        counter = 0,
        isMoving = false
    }
    function Entity:moveTo(toX, toY)
        self.isMoving = true
        self.x = toX
        self.y = toY
        self.toScreenX = (toX - 1) * Game.SpriteSize
        self.toScreenY = (toY - 1) * Game.SpriteSize
    end

    function Entity:update(dt)
        if self.screenX ~= self.toScreenX then
            if self.screenX > self.toScreenX then
                self.screenX = self.screenX - 1
            else
                self.screenX = self.screenX + 1
            end
        end
        if self.screenY ~= self.toScreenY then
            if self.screenY > self.toScreenY then
                self.screenY = self.screenY - 1
            else
                self.screenY = self.screenY + 1
            end
        end
        if self.screenX == self.toScreenX and self.screenY == self.toScreenY then
            self.isMoving = false
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
