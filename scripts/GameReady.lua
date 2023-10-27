require "GameBase"
require "Image"
require "GamePlay"

GameReady = {}

setmetatable(GameReady, {
    __call = function(self, mImage, mCount, mStarted)
        ---@class GameReady:GameBase
        ---@field mImage Image
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

        ---comment
        ---@param parent GameDirector
        function obj:update(parent)
            local next = self
            if not self.mStarted then
                parent:startLoading();
                self.mStarted = true;
            end
            if self.mCount >= 120 then
                next = GamePlay()
            elseif self.mCount >= 60 then
                Framework:drawDebugString(1, 2, "GO!")
            else
                Framework:drawDebugString(2, 3, "Ready...")
            end
            parent:drawState()
            self.mImage:draw()
            self.mCount = self.mCount + 1
            Framework:drawDebugString(1, 1, "[READY] : ")
            return next
        end

        setmetatable(obj, {
            __index = GameBase(),
            __tostring = function() return "GameReady" end
        })
        return obj
    end
})
