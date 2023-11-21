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
            __index = self,
            __mul = self.__mul
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

function Matrix23:setTranslation(a)
    self.m00, self.m11 = 1, 1
    self.m01, self.m10 = 0, 0
    self.m02 = a.x;
    self.m12 = a.y;
end

function Matrix23:setRotation(r)
    local c = math.pi / 180
    r = r * c
    self.m00 = math.cos(r)
    self.m11 = self.m00
    self.m10 = math.sin(r)
    self.m01 = -self.m10
    self.m02 = 0
    self.m12 = 0
end

function Matrix23:setScaling(a)
    self.m00 = a.x
    self.m11 = a.y
    self.m10 = 0
    self.m01 = 0
    self.m02 = 0
    self.m12 = 0
end

function Matrix23:__mul(m)
    local tx = self.m00;
    local ty = self.m01;
    self.m00 = tx * m.m00 + ty * m.m10;
    self.m01 = tx * m.m01 + ty * m.m11;
    self.m02 = tx * m.m02 + ty * m.m12 + self.m02;
    tx = self.m10;
    ty = self.m11;
    self.m10 = tx * m.m00 + ty * m.m10;
    self.m11 = tx * m.m01 + ty * m.m11;
    self.m12 = tx * m.m02 + ty * m.m12 + self.m12;
    return self
end

function Matrix23:multiply(out, input)
    local tx = input.x
    out.x = self.m00 * tx + self.m01 * input.y + self.m02
    out.y = self.m10 * tx + self.m11 * input.y + self.m12
end
