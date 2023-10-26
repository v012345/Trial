--- 对应 GameDirector.cpp
require "Image"
require "Director"
require "ScenceBase"
GameDirector = {}
setmetatable(GameDirector, {
    __call = function(self, mode)
        ---@class GameDirector:ScenceBase
        local obj = {}
        self.mState = nil
        self.mStageID = 0
        self.mLife = 0
        self.mChild = GameReady()

        if mode == Director.Mode.MODE_1P then
            self.mStageID = 1
        end

        function obj:isA(what)
            return GameDirector == what
        end

        setmetatable(obj, { __index = ScenceBase() })
        return obj
    end
})
