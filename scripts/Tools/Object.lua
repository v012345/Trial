---@class Object
Object = {}
setmetatable(Object, {
    __call = function(self)
        ---@class Object
        local obj = {}
        setmetatable(obj, { __index = self })
        obj:_construct()
        return obj
    end
})
function Object:_construct() end

function Object:dump(where)
    local attri = {}
    local function collectAttri(who)
        if who then
            for key, value in pairs(who) do
                if not attri[key] then
                    if not string.match(key, "^_.+$") then
                        attri[key] = true
                    end
                end
            end
            local mt = getmetatable(who)
            if mt then
                collectAttri(mt.__index)
            end
        end
    end
    collectAttri(self)
    for key, _ in pairs(attri) do
        print(key, self[key])
    end
end
