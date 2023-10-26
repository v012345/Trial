--- 对应 State.cpp
local State = {}

---@class State
local mt = {}

---@return State
function State:new(mode)
    ---@class State
    local obj = {}
    setmetatable(obj, { __index = mt })
    return obj
end

function mt:update()

end

function mt:draw()
    if Director:getMode() == Enum.Mode.MODE_1P then
        Framework:string("单人", 20, 20)
    else
        Framework:string("双人", 20, 20)
    end
    Framework:string(tostring(Director:get))
end

function mt:destroy()
end

return State
