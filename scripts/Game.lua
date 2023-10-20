function Game:init()
    self.SpriteSize = 32
    ---@type StageData
    self.StageData = {}
    ---@type StageData
    self.Background = {}
    ---@type table<integer,Entity>
    self.BackgroundEntities = {}
    ---@type table<integer,Entity>
    self.ForegroundEntities = {}
    self.PreviousTime = {}
    ---@type Entity
    self.player = {}
    for i = 1, 10, 1 do
        self.PreviousTime[i] = Framework.time()
    end
    self.iCounter = 0
end

---@param path string
function Game:readStageData(path)
    local file = io.open(path, "r") or error("can't open " .. path)
    local rawStageData = file:read()
    while rawStageData do
        local row = {}
        for i = 1, #rawStageData do
            local char = string.sub(rawStageData, i, i)
            if char == " " then
                row[#row + 1] = Enum.ground
            elseif char == "#" then
                row[#row + 1] = Enum.wall
            elseif char == "p" then
                row[#row + 1] = Enum.player
            elseif char == "." then
                row[#row + 1] = Enum.goal
            elseif char == "o" then
                row[#row + 1] = Enum.box
            else
                error(string.format("wrong char '%s' at line %s:%s", char, #map + 1, i))
            end
        end
        self.StageData[#self.StageData + 1] = row
        rawStageData = file:read()
    end
    file:close()
    self:loadBackground()
    self:loadForeground()
end

---@private
function Game:loadBackground()
    for _, v in ipairs(self.StageData) do
        local row = {}
        for _, obj in ipairs(v) do
            if obj == Enum.box then
                row[#row + 1] = Enum.ground
            elseif obj == Enum.player then
                row[#row + 1] = Enum.ground
            else
                row[#row + 1] = obj
            end
        end
        self.Background[#self.Background + 1] = row
    end

    for y, xRow in ipairs(self.Background) do
        for x, obj in ipairs(xRow) do
            self.BackgroundEntities[#self.BackgroundEntities + 1] = CreateEntity({
                x = x,
                y = y,
                screenX = (x - 1) * self.SpriteSize,
                screenY = (y - 1) * self.SpriteSize,
                eRenderEntity = obj,
                isBackground = true
            })
        end
    end
end

function Game:dumpBackground()
    for _, v in ipairs(self.Background) do
        for _, o in ipairs(v) do
            io.write(o)
        end
        io.write("\n")
    end
end

---@private
function Game:loadForeground()
    for y, v in ipairs(self.StageData) do
        for x, obj in ipairs(v) do
            if obj == Enum.box or obj == Enum.player then
                local box = {
                    x = x,
                    y = y,
                    screenX = (x - 1) * self.SpriteSize,
                    screenY = (y - 1) * self.SpriteSize,
                    eRenderEntity = obj,
                    isBackground = false
                }
                local entity = CreateEntity(box)
                if obj == Enum.player then
                    self.player = entity
                end
                self.ForegroundEntities[#self.ForegroundEntities + 1] = entity
            end
        end
    end
end

function Game:dumpMapWithEmoji()
    local EmojiEntity = {
        [Enum.box] = "📦",
        [Enum.empty] = "⬜",
        [Enum.goal] = "🔴",
        [Enum.ground] = "⬛",
        [Enum.player] = "😁",
        [Enum.wall] = "🧱",
    }
    local EmojiMap = {}
    for _, xRow in ipairs(self.Background) do
        local row = {}
        for _, obj in ipairs(xRow) do
            row[#row + 1] = EmojiEntity[obj]
        end
        EmojiMap[#EmojiMap + 1] = row
    end

    for _, entity in ipairs(self.ForegroundEntities) do
        EmojiMap[entity.y][entity.x] = EmojiEntity[entity.eRenderEntity]
    end
    for _, xRow in ipairs(EmojiMap) do
        for _, emoji in ipairs(xRow) do
            io.write(emoji)
        end
        io.write("\n")
    end
end

function Game:drawBackground()
    for _, renderObj in ipairs(self.BackgroundEntities) do
        renderObj:draw()
    end
end

function Game:update()
    self:drawBackground()
    self:drawForeground()
end

function Game:drawForeground()
    for _, renderObj in ipairs(self.ForegroundEntities) do
        renderObj:update()
    end
end

function Game:loadRenderImage(path)
    local png = ReadPngFile(path)
    local function slice(IMAGE, fromX, toX)
        local res = {}
        for _, xRow in ipairs(IMAGE) do
            local row = {}
            for x, argb in ipairs(xRow) do
                if fromX <= x and x <= toX then
                    row[#row + 1] = argb
                end
            end
            res[#res + 1] = row
        end
        return res
    end
    local empty = {}
    for _ = 1, self.SpriteSize do
        local row = {}
        for _ = 1, self.SpriteSize do
            row[#row + 1] = 0
        end
        empty[#empty + 1] = row
    end
    RenderEntity[Enum.player] = slice(png, 1, self.SpriteSize)
    RenderEntity[Enum.wall] = slice(png, 1 + self.SpriteSize, self.SpriteSize * 2)
    RenderEntity[Enum.box] = slice(png, 1 + self.SpriteSize * 2, self.SpriteSize * 3)
    RenderEntity[Enum.goal] = slice(png, 1 + self.SpriteSize * 3, self.SpriteSize * 4)
    RenderEntity[Enum.ground] = slice(png, 1 + self.SpriteSize * 4, self.SpriteSize * 5)
    RenderEntity[Enum.empty] = empty
end

function Game:dealInput()
    if Framework.isKeyOn(Keyboard.A) then
        self.player:move(-1, 0)
    end
    if Framework.isKeyOn(Keyboard.S) then
        self.player:move(0, 1)
    end
    if Framework.isKeyOn(Keyboard.D) then
        self.player:move(1, 0)
    end
    if Framework.isKeyOn(Keyboard.W) then
        self.player:move(0, -1)
    end
    if Framework.isKeyOn(Keyboard.Q) then

    end
    if Framework.isKeyOn(Keyboard.R) then

    end
end
