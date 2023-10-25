--- 对应 State.cpp
local State = {}

---@class State
local mt = {}

---@return State
function State:new(mode)
    ---@class State
    local obj = {}
    setmetatable(obj, { __index = mt })
    if mode == Enum.Mode.MODE_1P then
        obj.iStageID = 1
    else
        obj.iStageID = 0
    end
    obj.tChild = (require "Sequence.Game.Ready"):new()
    return obj
end

function mt:update(parent)
    local next = self
    local nextChild = self.tChild:update(self)
    if nextChild ~= next then

    end
    return next
end

function mt:state()

end

function mt:mode()
    local p = require "State"
    if p:mode() == Enum.Mode.MODE_1P then
        return Enum.Mode.MODE_1P
    end
    return Enum.Mode.MODE_2P
end

function mt:drawState()

end

function mt:startLoading()

end

function mt:destroy()
end

return State
