--- 对应 ScenceGame.cpp
-- require "State"
-- require "GameReady"
require "GameStage"
require "GameMenu"
ScenceGame = {
    FINAL_STAGE = 2,
    INITIALI_LIFE_NUMBER = 2
}
setmetatable(ScenceGame, {
    __call = function(self, mode)
        ---@class ScenceGame:ScenceBase
        ---@field mState GameStage
        ---@field mView GameMenu
        local obj = {}
        obj.mState = GameStage(mode)
        obj.mStageID = 0
        obj.mLife = 0
        obj.mWinner = 0
        obj.mView = nil
        -- obj.mChild = GameReady()

        if mode == Director.Mode.MODE_1P then
            obj.mStageID = 1
        end

        function obj:isA(what)
            return ScenceGame == what
        end

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

        function obj:startLoading()
            self.mState = State(self.mStageID)
        end

        function obj:drawState()
            self.mState:draw()
        end

        function obj:state()
            return self.mState
        end

        function obj:winner()
            return self.mWinner
        end

        function obj:hasFinalStageCleared()
            return self.mStageID > ScenceGame.FINAL_STAGE
        end

        function obj:lifeNumber()
            return self.mLife
        end

        function obj:setWinner(id)
            self.mWinner = id
        end

        function obj:goToNextStage()
            self.mStageID = self.mStageID + 1
        end

        function obj:mode()
            local r = Director.Mode.MODE_NONE;
            if Director:mode() == Director.Mode.MODE_1P then
                r = Director.Mode.MODE_1P
            elseif Director:mode() == Director.Mode.MODE_2P then
                r = Director.Mode.MODE_2P
            else
                error("MODE_NONE")
            end

            return r;
        end

        setmetatable(obj, {
            __index = ScenceBase(),
            __tostring = function() return "ScenceGame" end
        })
        return obj
    end
})
