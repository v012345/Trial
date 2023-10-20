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
    self.player = {}
    for y, v in ipairs(map) do
        for x, obj in ipairs(v) do
            if obj == ENUM.box then
                local box = {
                    x = x,
                    y = y
                }
                self.box[#self.box + 1] = box
            elseif obj == ENUM.player then
                self.player.x = x
                self.player.y = y
            end
        end
    end
end

function Game:dumpEntities()
    io.write(string.format("player at (%s,%s)\n", self.player.x, self.player.y))
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
    EmojiMap[self.player.y][self.player.x] = EmojiEntity[ENUM.player]
    for _, xRow in ipairs(EmojiMap) do
        for _, emoji in ipairs(xRow) do
            io.write(emoji)
        end
        io.write("\n")
    end
end

function Game:drawBackground()

end

function Game:loadRenderImage(path)

end
