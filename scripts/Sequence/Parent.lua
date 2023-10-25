-- 是一个单例

---@class Parent
---@field tChild any
Parent = {}
function Parent:init()
    self.eMode = Enum.Mode.MODE_NONE
    self.tChild = Title:new();
end

function Parent:update()
    local nextChild = self.tChild:update(self);
    if nextChild ~= self.tChild then
        -- 说明还场景了?
        self.tChild:destroy()
        self.tChild = nextChild;
    end
end

function Parent:getMode()
    return self.eMode
end

function Parent:setMode(mode)
    self.eMode = mode
end

function Parent:mode()
    return self:getMode()
end

Parent:init()
