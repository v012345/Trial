require "GameBase"
require "Image"

GameReady = {}

setmetatable(GameReady, {
    __call = function(self, mImage, mCount, mStarted)
        ---@class GameReady:GameBase
        local obj = {}
        function obj:isA(what)
            if GameReady == what then
                return true
            else
                local super = getmetatable(self)
                if super then
                    return super.__index:isA(what)
                else
                    return false
                end
            end
        end

        obj.mImage = mImage or Image(CMAKE_SOURCE_DIR .. "res/dummy.png")
        obj.mCount = mCount or 0
        obj.mStarted = mStarted or false
        function obj:update(parent)
            local next = self
            self.mImage:draw()
            return next
        end

        setmetatable(obj, {
            __index = GameBase(),
            __tostring = function() return "GameReady" end
        })
        return obj
    end
})
