---@class SpriteManager
---@field private getBlock function
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
        canControl = true
    }
    function res:update()
        local now = Framework.time()
        if now - self.lastUpdateAt > 42 then
            self.age = self.age + 1
            self.lastUpdateAt = now
            self.looklike = self.actions[self.age % 10 + 1]
        end
        Framework:draw(self.looklike, math.floor(self.x + 0.5), math.floor(self.y + 0.5), false)
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
        self.x = self.x + dx * self.velocity
        self.y = self.y + dy * self.velocity
    end

    function res:moveTo(toX, toY, canIntercept)

    end

    local r = {}
    for i = 0, 9 do
        r[#r + 1] = self:getBlock(png, i * data.cell_width, 5 * data.cell_height, (i + 1) * data.cell_width,
            6 * data.cell_height)
    end
    res.looklike = r[1]
    res.actions = r

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
