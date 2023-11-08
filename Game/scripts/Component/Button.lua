require "Object"
---@class Button:Object
---@field _render Image[]
Button = {
    status = {
        normal = 1,
        pressed = 2,
        disable = 3
    }
}

setmetatable(Button, {
    __call = function(self, normal, pressed, disable)
        ---@class Button
        local obj = {}
        setmetatable(obj, { __index = self })
        obj:init(normal, pressed, disable)
        return obj
    end,
    __index = Object()
})

function Button:init(normal, pressed, disable)
    pressed = pressed or normal
    disable = disable or normal
    self._render = {}
    for i, v in ipairs({ normal, pressed, disable }) do
        self._render[i] = Image(v)
    end
    self._status = Button.status.normal
    self._x = 0
    self._y = 0
end

function Button:update()
    self._render[self._status]:draw(self._x, self._y)
end
