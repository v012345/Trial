require "ObjectDynamic"
---@class ObjectWorker:ObjectDynamic
ObjectWorker = {}
setmetatable(ObjectWorker, {
    __call = function(self)
        ---@class ObjectWorker
        local obj = {}
        setmetatable(obj, {
            __index = self,
            __tostring = function() return "ObjectWorkerObject" end
        })
        return obj
    end,
    __index = ObjectDynamic(),
    __tostring = function() return "ObjectWorkerClass" end
})
