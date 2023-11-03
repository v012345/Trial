require "Object"
ScenceBase = {}
setmetatable(ScenceBase, {
    __call = function(self)
        ---@class ScenceBase:Object
        local obj = {}
        function obj:update(p)
            error("plz implementions update")
        end

        setmetatable(obj, { __index = self })
        return obj
    end,
    __index = Object()
})
