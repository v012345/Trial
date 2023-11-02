GameStage = {}
require "Tools.ParseXML"
setmetatable(GameStage, {
    __call = function(self, mode)
        ---@class GameStage
        ---@field mImage Image
        local obj = {}
        function obj:isA(what)
            if GameStage == what then
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

        obj.mBackground = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.png")
        obj.mBgConfig = ParseXML(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.xml")
        obj.mSprite = obj.mBgConfig.mData.children
        obj.mRedPoint = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/red.png")
        obj.mX = 0
        obj.mY = 0
        function obj:draw()
            self.mBackground:draw(0, 0, self.mSprite[1].attributes.x, self.mSprite[1].attributes.y,
                self.mSprite[1].attributes.width, self.mSprite[1].attributes.height
            )
            self.mRedPoint:draw(self.mX, self.mY, 0, 0, 20, 20)
        end

        function obj:update()
            self.mX = self.mX + 1
            if self.mX > 300 then
                self.mY = self.mY + 1
                if self.mY > 300 then
                    self.mX = 0
                    self.mY = 0
                end
            end
            self:draw()
        end

        function obj:isClear()
            return false
        end

        setmetatable(obj, {
            __index = Object(),
            __tostring = function() return "GameStage" end
        })
        return obj
    end
})
