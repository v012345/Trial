--- 对应 Play.cpp
local Play = {}

---@class Play
local mt = {}

---@return Play
function Play:new()
    ---@class Play
    local obj = {}
    setmetatable(obj, { __index = mt })
    return obj
end

---comment
---@param parent GameParent
function mt:update(parent)
    local next = self
    local state = parent:state()
    state:update()
    state:draw()
    return next
end

function mt:destroy()
end

return Play
