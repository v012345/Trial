--- 对应 GameDirector.cpp
require "State"
require "GameReady"
-- require "ScenceBase"
GameDirector = {
    FINAL_STAGE = 2,
    INITIALI_LIFE_NUMBER = 2
}
setmetatable(GameDirector, {
    __call = function(self, mode)
        ---@class GameDirector:ScenceBase
        ---@field mState State
        local obj = {}
        obj.mState = nil
        obj.mStageID = 0
        obj.mLife = 0
        obj.mWinner = 0
        obj.mChild = GameReady()

        if mode == Director.Mode.MODE_1P then
            obj.mStageID = 1
        end

        function obj:isA(what)
            return GameDirector == what
        end

        function obj:update(parent)
            local next = self
            local nextChild = self.mChild:update(self)
            if nextChild ~= next then
                if nextChild:isA(GameBase) then
                    self.mChild = nextChild
                else
                    next = nextChild
                end
            end
            return next
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
            return self.mStageID > GameDirector.FINAL_STAGE
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
            if Director:instance():mode() == Director.Mode.MODE_1P then
                r = Director.Mode.MODE_1P
            elseif Director:instance():mode() == Director.Mode.MODE_2P then
                r = Director.Mode.MODE_2P
            else
                error("MODE_NONE")
            end

            return r;
        end

        setmetatable(obj, {
            __index = ScenceBase(),
            __tostring = function() return "GameDirector" end
        })
        return obj
    end
})
