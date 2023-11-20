require "Object"
---@class Matrix22:Object
Matrix22 = {
    __parent = Object
}

setmetatable(Matrix22, {
    __call = function(self, e00, e01, e10, e11)
        ---@class Matrix22
        local obj = {}
        setmetatable(obj, {
            __index = self
        })
        obj:_construct(e00, e01, e10, e11)
        return obj
    end,
    __index = Matrix22.__parent
})

function Matrix22:_construct(e00, e01, e10, e11)
    Matrix22.__parent._construct(self)
    e00 = e00 or 0
    e01 = e01 or 0
    e10 = e10 or 0
    e11 = e11 or 0
    self.m00 = e00
    self.m01 = e01
    self.m10 = e10
    self.m11 = e11
end

function Matrix22:multiply(out, input)
    local tx = input.x
    out.x = self.m00 * tx + self.m01 * input.y
    out.y = self.m10 * tx + self.m11 * input.y
end
