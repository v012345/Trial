--- 对应 GameParent.cpp
local INITIALI_LIFE_NUMBER = 2
local FINAL_STAGE = 2
local GameParent = {}

---@class GameParent
---@field tChild any
local mt = {}

---@return GameParent
function GameParent:new(mode)
    ---@class GameParent
    local obj = {}
    setmetatable(obj, { __index = mt })
    if mode == Enum.Mode.MODE_1P then
        obj.iStageID = 1
    else
        obj.iStageID = 0
    end
    obj.tChild = (require "Sequence.Game.Ready"):new()
    obj.tState = {}
    obj.iLife = INITIALI_LIFE_NUMBER
    return obj
end

function mt:update(parent)
    local next = self
    local nextChild = self.tChild:update(self)
    if nextChild ~= next then
        if nextChild:getType() == Enum.UIType.View then
            self.tChild = nextChild
        else
            next = nextChild
        end
    end
    return next
end

function mt:state()
    return self.tState
end

function mt:mode()
    if Director:mode() == Enum.Mode.MODE_1P then
        return Enum.Mode.MODE_1P
    end
    return Enum.Mode.MODE_2P
end

function mt:drawState()
    self.tState:draw()
end

function mt:startLoading()
    self.tState = (require "State"):new(self.iStageID)
end

function mt:lifeNumber()
    return self.iLife
end

function mt:destroy()
end

return GameParent
