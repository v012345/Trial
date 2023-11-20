require "Object"
---@class Vector2:Object
Vector2 = {
    __parent = Object
}

setmetatable(Vector2, {
    __call = function(self, x, y)
        ---@class Vector2
        local obj = {}
        setmetatable(obj, {
            __index = self,
            __sub = self.__sub,
            __add = self.__add
        })
        obj:_construct(x, y)
        return obj
    end,
    __index = Vector2.__parent
})

function Vector2:_construct(x, y)
    Vector2.__parent._construct(self)
    x = x or 0
    y = y or 0
    self.x = x
    self.y = y
end

function Vector2:clone()
    return Vector2(self.x, self.y)
end

function Vector2:__sub(v)
    if v.isA and v:isA(Vector2) then
        self.x = self.x - v.x
        self.y = self.y - v.y
    end
    return self
end

function Vector2:__add(v)
    if v.isA and v:isA(Vector2) then
        self.x = self.x + v.x
        self.y = self.y + v.y
    end
    return self
end

function Vector2:setAdd(a, b)
    self.x = a.x + b.x;
    self.y = a.y + b.y;
end

function Vector2:setSub(a, b)
    self.x = a.x - b.x;
    self.y = a.y - b.y;
end

function Vector2:setMul(a, b)
    self.x = a.x * b.x;
    self.y = a.y * b.y;
end

function Vector2:setInterporation(a, ab, ac, u, v)
    self.x = a.x + ab.x * u + ac.x * v;
    self.y = a.y + ab.y * u + ac.y * v;
end
