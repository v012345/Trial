Sprite = {}
local mt = {}


function Sprite:new(SpriteConfig)
    local object = {}
    setmetatable(object, { __index = mt })
    return object
end
