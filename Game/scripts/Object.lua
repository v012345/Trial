Object = {}
setmetatable(Object, {
    __call = function()
        ---@class Object
        local obj = {}
        function obj:isA(what)
            return Object == what
        end

        return obj
    end
})
