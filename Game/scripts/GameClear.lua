---@class GameClear
GameClear = {}
setmetatable(GameClear, {
    __call = function(self)
        ---@class GameClear
        ---@field _mImage Image
        local obj = {}
        setmetatable(obj, {
            __index = self,
            __tostring = function() return "GameClearObject" end
        })
        obj._mImage = Image(CMAKE_CURRENT_SOURCE_DIR .. "/res/console.png")
        return obj
    end,
    __index = Object(),
    __tostring = function() return "GameClearClass" end
})
function GameClear:update()
    self._mImage:draw()
    Framework:drawDebugString(10, 10, "congratulations! press space to continue")
end
