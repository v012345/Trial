require "ScenceBase"
require "ScenceGame"
ScenceTitle = {}
setmetatable(ScenceTitle, {
    __call = function(self)
        ---@class ScenceTitle:ScenceBase
        local obj = {}
        obj.mImage = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/console.png")
        obj.mStageId = 0

        function obj:isA(what)
            return ScenceTitle == what
        end

        function obj:update()
            if Framework:isKeyTriggered(Enum.Keyboard.S) then
                self.mStageId = self.mStageId - 1
                if self.mStageId < 0 then
                    self.mStageId = 1
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.W) then
                self.mStageId = self.mStageId + 1
                if self.mStageId > 1 then
                    self.mStageId = 0
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.Space) then
                if self.mStageId == 0 then
                    Director:changeScence(ScenceGame(Enum.Mode.MODE_1P))
                elseif self.mStageId == 1 then
                    Director:changeScence(ScenceGame(Enum.Mode.MODE_2P))
                else
                    error("no possible")
                end
            end
            self.mImage:draw()
            Framework:drawDebugString(0, 0, "[Title] : boom man, press space for confirm")
            Framework:drawDebugString(1, 2, "one player");
            Framework:drawDebugString(1, 3, "two players");
            Framework:drawDebugString(0, self.mStageId + 2, ">");
        end

        setmetatable(obj, {
            __index = ScenceBase(),
            __tostring = function() return "ScenceTitle" end
        })
        return obj
    end
})
