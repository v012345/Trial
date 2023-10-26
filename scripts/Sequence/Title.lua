--- 对应 Title.cpp
Title = {}

---@class Title
local mt = {}

---@return Title
function Title:new()
    ---@class Title
    local obj = {}
    setmetatable(obj, { __index = mt })
    obj.tImage = Image:new(CMAKE_SOURCE_DIR .. "res/dummy.png")
    obj.iCursorPosistion = 1
    return obj
end

---comment
---@param director Director
---@return GameParent|Title
function mt:update(director)
    local next = self
    if Framework:isKeyTriggered(Enum.Keyboard.S) then
        self.iCursorPosistion = self.iCursorPosistion - 1
        if self.iCursorPosistion < 1 then
            self.iCursorPosistion = 2
        end
    elseif Framework:isKeyTriggered(Enum.Keyboard.W) then
        self.iCursorPosistion = self.iCursorPosistion + 1
        if self.iCursorPosistion > 2 then
            self.iCursorPosistion = 1
        end
    elseif Framework:isKeyTriggered(Enum.Keyboard.Space) then
        next = (require "Sequence.Game.GameParent"):new(director:mode())
        if self.iCursorPosistion == 1 then
            director:setMode(Enum.Mode.MODE_1P)
        elseif self.iCursorPosistion == 2 then
            director:setMode(Enum.Mode.MODE_2P)
        else
            error("no possible")
        end
    end
    self.tImage:draw()
    Framework:drawDebugString(1, 1, "[title] : boom man, press space for confirm")
    Framework:drawDebugString(2, 3, "one player");
    Framework:drawDebugString(2, 4, "two players");
    Framework:drawDebugString(1, self.iCursorPosistion + 2, ">");
    return next
end

function mt:destroy()
end
