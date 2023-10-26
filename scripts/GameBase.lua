require "Object"
GameBase = {}
setmetatable(GameBase, {
    __call = function(self)
        ---@class GameBase:Object
        local obj = {}
        function obj:isA(what)
            return GameBase == what
        end

        function obj:update(p)
            -- return self:update(p)
            error("plz implementions update")
        end

        setmetatable(obj, { __index = Object() })
        return obj
    end
})
