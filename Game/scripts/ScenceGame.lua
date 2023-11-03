require "GameStage"
require "GameMenu"
ScenceGame = {}
setmetatable(ScenceGame, {
    __call = function(self, stageId)
        ---@class ScenceGame:ScenceBase
        ---@field mState GameStage
        ---@field mView GameMenu
        local obj = {}
        obj.mState = GameStage(stageId)
        obj.mView = nil


        function obj:update()
            if Framework:isKeyTriggered(Enum.Keyboard.Q) then
                Director:changeScence(ScenceTitle())
                return
            end
            if self.mView then
                self.mState:draw()
                self.mView:update()
                return
            end
            if Framework:isKeyTriggered(Enum.Keyboard.E) then
                self.mView = GameMenu(function() self.mView = nil end)
            end
            self.mState:update()
        end

        setmetatable(obj, {
            __index = self,
            __tostring = function() return "ScenceGame" end
        })
        return obj
    end,
    __index = ScenceBase(),
    __tostring = function() return "ScenceGame" end
})
