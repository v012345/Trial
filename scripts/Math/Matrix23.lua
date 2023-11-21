require "Object"
---@class Matrix23:Object
Matrix23 = {
    __parent = Object
}

setmetatable(Matrix23, {
    __call = function(self, e00, e01, e02, e10, e11, e12)
        ---@class Matrix23
        local obj = {}
        setmetatable(obj, {
            __index = self
        })
        obj:_construct(e00, e01, e02, e10, e11, e12)
        return obj
    end,
    __index = Matrix23.__parent
})

function Matrix23:_construct(e00, e01, e02, e10, e11, e12)
    Matrix23.__parent._construct(self)
    e00 = e00 or 0
    e01 = e01 or 0
    e02 = e02 or 0
    e10 = e10 or 0
    e11 = e11 or 0
    e12 = e12 or 0
    self.m00 = e00
    self.m01 = e01
    self.m02 = e02
    self.m10 = e10
    self.m11 = e11
    self.m12 = e12
end

function Matrix23:multiply(out, input)
    local tx = input.x
    out.x = self.m00 * tx + self.m01 * input.y + self.m02
    out.y = self.m10 * tx + self.m11 * input.y + self.m12
end
