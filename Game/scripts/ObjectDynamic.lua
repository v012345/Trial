---@class ObjectDynamic:Object
ObjectDynamic = {}
setmetatable(ObjectDynamic, {
    __call = function(self)
        ---@class ObjectDynamic
        local obj = {}
        setmetatable(obj, {
            __index = self,
            __tostring = function() return "ObjectDynamicObject" end
        })
        return obj
    end,
    __index = Object(),
    __tostring = function() return "ObjectDynamicClass" end
})
