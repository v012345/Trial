--- 对应 Title.cpp
-- require "Image"
-- require "ScenceBase"
require "GameDirector"
Title = {}
setmetatable(Title, {
    __call = function(self)
        ---@class Title:ScenceBase
        local obj = {}
        obj.mImage = Image(CMAKE_SOURCE_DIR .. "res/dummy.png")
        obj.mCursorPosistion = 1

        function obj:isA(what)
            return Title == what
        end

        ---comment
        ---@param director Director
        function obj:update(director)
            local next = self
            if Framework:isKeyTriggered(Enum.Keyboard.S) then
                self.mCursorPosistion = self.mCursorPosistion - 1
                if self.mCursorPosistion < 1 then
                    self.mCursorPosistion = 2
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.W) then
                self.mCursorPosistion = self.mCursorPosistion + 1
                if self.mCursorPosistion > 2 then
                    self.mCursorPosistion = 1
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.Space) then
                next = GameDirector(director:mode())
                if self.mCursorPosistion == 1 then
                    director:setMode(Enum.Mode.MODE_1P)
                elseif self.mCursorPosistion == 2 then
                    director:setMode(Enum.Mode.MODE_2P)
                else
                    error("no possible")
                end
            end
            self.mImage:draw()
            Framework:drawDebugString(1, 1, "[title] : boom man, press space for confirm")
            Framework:drawDebugString(2, 3, "one player");
            Framework:drawDebugString(2, 4, "two players");
            Framework:drawDebugString(1, self.mCursorPosistion + 2, ">");
            Framework:drawDebugString(20, 5, "Title.lua")
            return next
        end

        setmetatable(obj, {
            __index = ScenceBase(),
            __tostring = function() return "Title" end
        })
        return obj
    end
})
