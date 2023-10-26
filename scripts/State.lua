require "Object"
State = {}
setmetatable(State, {
    __call = function(self)
        ---@class State:Object
        local obj = {}
        function obj:isA(what)
            return State == what
        end

        function obj:update(p)
            -- return self:update(p)
            error("plz implementions update")
        end

        setmetatable(obj, { __index = Object() })
        return obj
    end
})
