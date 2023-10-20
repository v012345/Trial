require "Player"
Game.SpriteSize = 32
function Game:drawImage(IMAGE, atX, atY, isBG)
    for y, row in ipairs(IMAGE) do
        for x, argb in ipairs(row) do
            if isBG then
                Impl:vram(atX + x - 1, atY + y - 1, argb)
            else
                local srcA = (argb & 0xff000000) >> 24
                local srcR = argb & 0xff0000;
                local srcG = argb & 0x00ff00;
                local srcB = argb & 0x0000ff;
                local dst = Impl:colorAt(atX + x - 1, atY + y - 1)
                local dstR = dst & 0xff0000;
                local dstG = dst & 0x00ff00;
                local dstB = dst & 0x0000ff;
                local r = (srcR - dstR) * srcA // 255 + dstR;
                local g = (srcG - dstG) * srcA // 255 + dstG;
                local b = (srcB - dstB) * srcA // 255 + dstB;
                -- 如果大于255则设置为255
                r = (r > 0xff0000) and 0xff0000 or r;
                g = (g > 0x00ff00) and 0x00ff00 or g;
                b = (b > 0x0000ff) and 0x0000ff or b;
                dst = (r & 0xff0000) | (g & 0x00ff00) | b;
                Impl:vram(atX + x - 1, atY + y - 1, dst)
            end
        end
    end
end

function Game:loadBackground(map)
    self.background = {}
    for _, v in ipairs(map) do
        local row = {}
        for _, obj in ipairs(v) do
            if obj == ENUM.box then
                row[#row + 1] = ENUM.ground
            elseif obj == ENUM.player then
                row[#row + 1] = ENUM.ground
            else
                row[#row + 1] = obj
            end
        end
        self.background[#self.background + 1] = row
    end
    self.renderBackground = {}
    for y, xRow in ipairs(self.background) do
        for x, obj in ipairs(xRow) do
            self.renderBackground[#self.renderBackground + 1] = {
                screenX = (x - 1) * self.SpriteSize,
                screenY = (y - 1) * self.SpriteSize,
                obj = obj
            }
        end
    end
end

function Game:dumpBackground()
    for _, v in ipairs(self.background) do
        for _, o in ipairs(v) do
            io.write(o)
        end
        io.write("\n")
    end
end

function Game:loadEntities(map)
    self.box = {}
    for y, v in ipairs(map) do
        for x, obj in ipairs(v) do
            if obj == ENUM.box then
                local box = {
                    x = x,
                    y = y,
                    screenX = (x - 1) * self.SpriteSize,
                    screenY = (y - 1) * self.SpriteSize
                }
                self.box[#self.box + 1] = box
            elseif obj == ENUM.player then
                Player.x = x
                Player.y = y
                Player.screenX = (x - 1) * self.SpriteSize
                Player.screenY = (y - 1) * self.SpriteSize
            end
        end
    end
end

function Game:dumpEntities()
    io.write(string.format("player at (%s,%s)\n", Player.x, Player.y))
    for _, box in ipairs(self.box) do
        io.write(string.format("box at (%s,%s)\n", box.x, box.y))
    end
end

function Game:dumpMapWithEmoji()
    local EmojiEntity = {
        [ENUM.box] = "📦",
        [ENUM.empty] = "⬜",
        [ENUM.goal] = "🔴",
        [ENUM.ground] = "⬛",
        [ENUM.player] = "😁",
        [ENUM.wall] = "🧱",
    }
    local EmojiMap = {}
    for _, xRow in ipairs(self.background) do
        local row = {}
        for _, obj in ipairs(xRow) do
            row[#row + 1] = EmojiEntity[obj]
        end
        EmojiMap[#EmojiMap + 1] = row
    end

    for _, box in ipairs(self.box) do
        EmojiMap[box.y][box.x] = EmojiEntity[ENUM.box]
    end
    EmojiMap[Player.y][Player.x] = EmojiEntity[ENUM.player]
    for _, xRow in ipairs(EmojiMap) do
        for _, emoji in ipairs(xRow) do
            io.write(emoji)
        end
        io.write("\n")
    end
end

function Game:drawBackground()
    for _, renderObj in ipairs(self.renderBackground) do
        self:drawImage(self.renderEntity[renderObj.obj], renderObj.screenX, renderObj.screenY, true)
    end
end

function Game:drawEntities()
    for _, box in ipairs(self.box) do
        self:drawImage(self.renderEntity[ENUM.box], box.screenX, box.screenY, false)
    end
end

function Game:loadRenderImage(path)
    local png = ReadPngFile(path)
    local function slice(IMAGE, fromX, toX)
        local res = {}
        for y, xRow in ipairs(IMAGE) do
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
    for i = 1, self.SpriteSize do
        local row = {}
        for j = 1, self.SpriteSize do
            row[#row + 1] = 0
        end
        empty[#empty + 1] = row
    end
    self.renderEntity = {}
    self.renderEntity[ENUM.player] = slice(png, 1, self.SpriteSize)
    self.renderEntity[ENUM.wall] = slice(png, 1 + self.SpriteSize, self.SpriteSize * 2)
    self.renderEntity[ENUM.box] = slice(png, 1 + self.SpriteSize * 2, self.SpriteSize * 3)
    self.renderEntity[ENUM.goal] = slice(png, 1 + self.SpriteSize * 3, self.SpriteSize * 4)
    self.renderEntity[ENUM.ground] = slice(png, 1 + self.SpriteSize * 4, self.SpriteSize * 5)
    self.renderEntity[ENUM.empty] = empty
    -- self:drawImage(self.renderEntity[ENUM.box], 0, 0, true)
end

function Game:dealInput()
    if Framework.isKeyOn(Keyboard.A) then
        Player:move(-1, 0)
    end
    if Framework.isKeyOn(Keyboard.S) then
        Player:move(0, 1)
    end
    if Framework.isKeyOn(Keyboard.D) then
        Player:move(1, 0)
    end
    if Framework.isKeyOn(Keyboard.W) then
        Player:move(0, -1)
    end
    if Framework.isKeyOn(Keyboard.Q) then

    end
    if Framework.isKeyOn(Keyboard.R) then

    end
end
