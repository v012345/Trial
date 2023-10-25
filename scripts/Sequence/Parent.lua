-- 是一个单例
Parent = {}
function Parent:init()
    self.eMode = Enum.Mode.MODE_NONE
    self.Child = Title:new();
end

function Parent:update()
    local nextChild = self.Child:update(self);
    if nextChild ~= self.Child then
        -- 说明还场景了?
        self.Child:destroy()
        self.Child = nextChild;
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
