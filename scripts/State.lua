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
    print("ddd")
    for i = 10, 100, 1 do
        for j = 10, 100, 1 do
            Screen:setColor(i, j, 0x0fff0000)
        end
    end
end

function mt:destroy()
end

return State
