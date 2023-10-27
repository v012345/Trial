require "GameBase"
require "Image"

GamePause = {}

setmetatable(GamePause, {
    __call = function(self)
        ---@class GamePause:GameBase
        local obj = {}
        function obj:isA(what)
            if GamePause == what then
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

        obj.mImage = Image(CMAKE_SOURCE_DIR .. "res/dummy.png")
        obj.mCursorPosistion = 1

        ---comment
        ---@param parent GameDirector
        function obj:update(parent)
            local next = self
            if Framework:isKeyTriggered(Enum.Keyboard.W) then
                self.mCursorPosistion = self.mCursorPosistion - 1
                if self.mCursorPosistion < 1 then
                    self.mCursorPosistion = 2
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.S) then
                self.mCursorPosistion = self.mCursorPosistion + 1
                if self.mCursorPosistion > 2 then
                    self.mCursorPosistion = 1
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.Space) then
                if self.mCursorPosistion == 1 then
                    next = GamePlay()
                elseif self.mCursorPosistion == 2 then
                    next = Title()
                end
            end
            parent:drawState()
            self.mImage:draw()

            Framework:drawDebugString(1, 1, "[pause]");
            Framework:drawDebugString(2, 3, "game continue");
            Framework:drawDebugString(2, 4, "return title");
            Framework:drawDebugString(1, self.mCursorPosistion + 2, ">");
            return next
        end

        setmetatable(obj, {
            __index = GameBase(),
            __tostring = function() return "GamePause" end
        })
        return obj
    end
})
