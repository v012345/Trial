--- 对应 Ready.cpp
local Ready = {}

---@class Ready
local mt = {}

---@return Ready
function Ready:new()
    ---@class Ready
    local obj = {}
    setmetatable(obj, { __index = mt })
    obj.tImage = Image:new(CMAKE_SOURCE_DIR .. "res/dummy.png")
    obj.iCount = 0
    obj.bStarted = false
    return obj
end

---@param parent GameParent
---@return Ready
function mt:update(parent)
    local next = self
    if not self.bStarted then
        parent:startLoading();
        self.bStarted = true;
    end
    if self.iCount >= 120 then
    elseif self.iCount >= 60 then
        Framework:drawDebugString(1, 2, "GO!")
    else
        Framework:drawDebugString(1, 2, "Ready...")
    end
    parent:drawState();
    self.tImage:draw();
    Framework:drawDebugString(1, 1, "[READY] : ")
    self.iCount = self.iCount + 1
    return next
end

function mt:destroy()
end

return Ready
