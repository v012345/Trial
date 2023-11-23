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
    self.m00, self.m10 = e00, e10
    self.m01, self.m11 = e01, e11
    self.m02, self.m12 = e02, e12
end

function Matrix23:setTranslation(a)
    self.m00, self.m10 = 1, 0
    self.m01, self.m11 = 0, 1
    self.m02, self.m12 = a.x, a.y
end

-- 从 x 转 y, 与 cpp 不同, 我觉得是 cpp 取了
function Matrix23:setRotation(r)
    r = r * math.pi / 180
    local c = math.cos(r)
    local s = math.sin(r)
    self.m00, self.m10 = c, s
    self.m01, self.m11 = -s, c
    self.m02, self.m12 = 0, 0
end

function Matrix23:setScaling(a)
    self.m00, self.m10 = a.x, 0
    self.m01, self.m11 = 0, a.y
    self.m02, self.m12 = 0, 0
end

-- ok
function Matrix23:scale(a)
    self.m00, self.m10 = self.m00 * a.x, self.m10 * a.y
    self.m01, self.m11 = self.m01 * a.x, self.m11 * a.y
    self.m02, self.m12 = self.m02 * a.x, self.m12 * a.y
end

-- ok
function Matrix23:translate(a)
    self.m02 = self.m02 + a.x
    self.m12 = self.m12 + a.y
end

-- ok
function Matrix23:rotate(r)
    local c = math.cos(r * math.pi / 180);
    local s = math.sin(r * math.pi / 180);
    self.m00, self.m10 = self.m00 * c - self.m10 * s, self.m00 * s + self.m10 * c;
    self.m01, self.m11 = self.m01 * c - self.m11 * s, self.m01 * s + self.m11 * c;
    self.m02, self.m12 = self.m02 * c - self.m12 * s, self.m02 * s + self.m12 * c;
end

function Matrix23:__mul(m)
    local m00, m10 = self.m00 * m.m00 + self.m10 * m.m01, self.m00 * m.m10 + self.m10 * m.m11
    local m01, m11 = self.m01 * m.m00 + self.m11 * m.m01, self.m01 * m.m10 + self.m11 * m.m11
    local m02, m12 = self.m02 * m.m00 + self.m12 * m.m01 + m.m02, self.m02 * m.m10 + self.m12 * m.m11 + m.m12
    self.m00, self.m10 = m00, m10
    self.m01, self.m11 = m01, m11
    self.m02, self.m12 = m02, m12
    return self
end

-- 没有什么问题
function Matrix23:multiply(out, input)
    local tx = input.x
    out.x = self.m00 * tx + self.m01 * input.y + self.m02
    out.y = self.m10 * tx + self.m11 * input.y + self.m12
end
