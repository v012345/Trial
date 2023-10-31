require "GameBase"
require "Image"
require "GamePause"
require "Enum"

GamePlay = {}

setmetatable(GamePlay, {
    __call = function(self)
        ---@class GamePlay:GameBase
        local obj = {}
        function obj:isA(what)
            if GamePlay == what then
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

        ---comment
        ---@param parent GameDirector
        ---@return GamePlay
        function obj:update(parent)
            local next = self
            local state = parent:state()
            local cleared = state:hasCleared()
            local die1P = not state:isAlive(0);
            local die2P = not state:isAlive(1);
            if Framework:isKeyTriggered(Enum.Keyboard.One) then
                die1P = true
            elseif Framework:isKeyTriggered(Enum.Keyboard.Two) then
                die2P = true
            elseif Framework:isKeyTriggered(Enum.Keyboard.Three) then
                cleared = true
            end
            if next == self and Framework:isKeyTriggered(Enum.Keyboard.P) then
                next = GamePause()
            end

            state:update()
            state:draw()
            return next
        end

        setmetatable(obj, {
            __index = GameBase(),
            __tostring = function() return "GamePlay" end
        })
        return obj
    end
})
