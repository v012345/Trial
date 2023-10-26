--- 对应 GameDirector.cpp
-- require "Image"
require "GameReady"
-- require "ScenceBase"
GameDirector = {}
setmetatable(GameDirector, {
    __call = function(self, mode)
        ---@class GameDirector:ScenceBase
        local obj = {}
        obj.mState = nil
        obj.mStageID = 0
        obj.mLife = 0
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
            print(nextChild)
            if nextChild ~= next then
                if nextChild:isA(GameBase) then
                    self.tChild = nextChild
                else
                    next = nextChild
                end
            end
            return next
        end

        setmetatable(obj, { __index = ScenceBase() })
        return obj
    end
})
