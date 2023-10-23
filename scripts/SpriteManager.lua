---@class SpriteManager
SpriteManager = SpriteManager or {}

---comment
---@param data Res_Sprite
function SpriteManager:create(data)
    local png = ReadPngFile(data.path)
    local res = {
        lastUpdateAt = 0,
        velocity = 1,
        age = 0,
        looklike = { {} },
        actions = {},
        x = 400,
        y = 0,
        oldX = 0,
        oldY = 0,
        canControl = true,
        eToward = Direction.Left
    }

    function res:update()
        local now = Framework.time()
        local status = "idle"
        if self.x ~= self.oldX or self.y ~= self.oldY then
            status = "walk"
        end
        local actions = self.actions[self.eToward][status]

        if now - self.lastUpdateAt > 125 then
            self.age = self.age + 1
            self.lastUpdateAt = now
            self.looklike = actions[self.age % #actions + 1]
        end
        Framework:draw(self.looklike, self.x, self.y, false)
        self.oldX = self.x
        self.oldY = self.y
    end

    function res:move(direction)
        local dx, dy = 0, 0
        if direction == Direction.Up then
            dy = -1
        elseif direction == Direction.Down then
            dy = 1
        elseif direction == Direction.Left then
            dx = -1
        elseif direction == Direction.Right then
            dx = 1
        end
        self.eToward = direction
        self.x = self.x + dx * self.velocity
        self.y = self.y + dy * self.velocity
    end

    function res:moveTo(toX, toY, canIntercept)

    end

    local actions = {}
    for _, direction in pairs(Direction) do
        actions[direction] = {}
        for state, config in pairs(data[direction]) do
            local sequence = {}
            for i = 0, config.frame - 1 do
                sequence[#sequence + 1] = self:getBlock(png, i * data.cell_width + 1,
                    (config.row - 1) * data.cell_height + 1,
                    (i + 1) * data.cell_width, config.row * data.cell_height)
            end
            actions[direction][state] = sequence
        end
    end
    res.looklike = actions[Direction.Left]["idle"][1]
    res.actions = actions

    return res
end

---@param PNG Image
---@param fromX integer 包括
---@param fromY integer 包括
---@param toX integer 不包括
---@param toY integer 不包括
---@return Image
function SpriteManager:getBlock(PNG, fromX, fromY, toX, toY)
    ---@type Image
    local piece = {}
    for y = fromY, toY - 1 do
        local row = {}
        for x = fromX, toX - 1 do
            row[#row + 1] = PNG[y][x]
        end
        piece[#piece + 1] = row
    end
    return piece
end
