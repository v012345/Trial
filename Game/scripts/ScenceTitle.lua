require "ScenceBase"
require "ScenceGame"
require "DataStage"
ScenceTitle = {}
setmetatable(ScenceTitle, {
    __call = function(self)
        ---@class ScenceTitle:ScenceBase
        local obj = {}
        obj.mImage = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/console.png")
        obj.mStageId = 1

        function obj:isA(what)
            return ScenceTitle == what
        end

        function obj:update()
            if Framework:isKeyTriggered(Enum.Keyboard.W) then
                self.mStageId = self.mStageId - 1
                if self.mStageId < 1 then
                    self.mStageId = DataStage:getStageCount()
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.S) then
                self.mStageId = self.mStageId + 1
                if self.mStageId > DataStage:getStageCount() then
                    self.mStageId = 1
                end
            elseif Framework:isKeyTriggered(Enum.Keyboard.Space) then
                Director:changeScence(ScenceGame(self.mStageId))
            end
            self.mImage:draw()
            Framework:drawDebugString(0, 0, "[Sokoban] : select stage , press space for confirm")
            for i = 1, DataStage:getStageCount() do
                Framework:drawDebugString(1, i + 1, "Stage " .. i);
            end
            Framework:drawDebugString(0, self.mStageId + 1, ">");
        end

        setmetatable(obj, {
            __index = ScenceBase(),
            __tostring = function() return "ScenceTitle" end
        })
        return obj
    end
})
