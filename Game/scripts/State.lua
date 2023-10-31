require "Object"
State = {
    STAGE_ID_2PLAYERS = 0
}
setmetatable(State, {
    __call = function(self, stageID)
        ---@class State:Object
        ---@field mImage Image
        local obj = {}
        obj.mImage = Image(CMAKE_SOURCE_DIR .. "res/dummy.png")
        function obj:isA(what)
            return State == what
        end

        function obj:update()
            self.mImage:draw()
        end

        function obj:hasCleared() end

        function obj:isAlive(playerID) end

        function obj:draw()
            Framework:string("我是状态", 30, 30)
        end

        function obj:setFire() end

        setmetatable(obj, { __index = Object() })
        return obj
    end
})
