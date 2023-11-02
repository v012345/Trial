require "Object"
ScenceBase = {}
setmetatable(ScenceBase, {
    __call = function(self)
        ---@class ScenceBase:Object
        local obj = {}
        function obj:isA(what)
            if ScenceBase == what then
                return true
            else
                local super = getmetatable(self)
                if super then
                    return super.__index:isA(what)
                else
                    return false
                end
            end
        end

        function obj:update(p)
            -- return self:update(p)
            error("plz implementions update")
        end

        setmetatable(obj, { __index = Object() })
        return obj
    end
})
