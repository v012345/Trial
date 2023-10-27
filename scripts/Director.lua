require "Title"
---@class Director:Object
Director = {
    mInstance = nil,
    MODE_NONE = 0,
    Mode = {
        MODE_1P = 1,
        MODE_2P = 2,
        MODE_NONE = 0,
    }
}
setmetatable(Director, {
    __call = function()
        if Director.mInstance then
            return Director.mInstance
        end
        ---@class Director
        local obj = {}
        function obj:isA(what)
            return Director == what
        end

        obj.mMode = Director.MODE_NONE
        obj.mChild = Title()

        function obj:update()
            local nextChild = self.mChild:update(self);
            if nextChild ~= self.mChild then
                self.mChild = nextChild;
            end
        end

        function obj:getMode()
            return self.mMode
        end

        function obj:setMode(mode)
            self.mMode = mode
        end

        function obj:mode()
            return self:getMode()
        end

        Director.mInstance = obj
        setmetatable(obj, {
            __index = Object(),
            __tostring = function() return "Director" end
        })
        return obj
    end
})
function Director:instance()
    return self.mInstance
end

Director()
