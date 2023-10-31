ScenceBase = {}
setmetatable(ScenceBase, {
    __call = function(self)
        ---@class ScenceBase:Object
        local obj = {}
        function obj:isA(what)
            return ScenceBase == what
        end

        function obj:update(p)
            -- return self:update(p)
            error("plz implementions update")
        end

        setmetatable(obj, { __index = Object() })
        return obj
    end
})
