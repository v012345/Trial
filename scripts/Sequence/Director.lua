-- 是一个单例

---@class Director
---@field tChild any
Director = {}
function Director:init()
    self.eMode = Enum.Mode.MODE_NONE
    self.tChild = Title:new();
end

function Director:update()
    local nextChild = self.tChild:update(self);
    if nextChild ~= self.tChild then
        -- 说明还场景了?
        self.tChild:destroy()
        self.tChild = nextChild;
    end
end

function Director:getMode()
    return self.eMode
end

function Director:setMode(mode)
    self.eMode = mode
end

function Director:mode()
    return self:getMode()
end

Director:init()
