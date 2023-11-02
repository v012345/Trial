GameStage = {}
require "Tools.ParseXML"
require "DataStage"
setmetatable(GameStage, {
    __call = function(self, mode)
        ---@class GameStage
        ---@field mImage Image
        ---@field mStage StageData
        local obj = {}
        obj.mStage = DataStage.stage[mode]
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

        function obj:getSprite(which)
            for _, sprite in ipairs(self.mSpriteConfig.children) do
                if sprite.attributes.name == which then
                    return {
                        x = sprite.attributes.x,
                        y = sprite.attributes.y,
                        w = sprite.attributes.width,
                        h = sprite.attributes.height
                    }
                end
            end
            error(which)
        end

        -- obj.mBackground = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.png")
        function obj:drawBackground()
            for y, row in ipairs(self.mStage.background) do
                for x, o in ipairs(row) do
                    if o == Enum.Obj.ground then
                        self.mSprite:draw((x - 1) * self.mGroundSprite.w, (y - 1) * self.mGroundSprite.h,
                            self.mGroundSprite.x,
                            self.mGroundSprite.y,
                            self.mGroundSprite.w,
                            self.mGroundSprite.h)
                    elseif o == Enum.Obj.wall then
                        self.mSprite:draw((x - 1) * self.mWallSprite.w, (y - 1) * self.mWallSprite.h,
                            self.mWallSprite.x,
                            self.mWallSprite.y,
                            self.mWallSprite.w,
                            self.mWallSprite.h)
                    end
                end
            end
            for _, goal in ipairs(self.mStage.goals) do
                self.mSprite:draw((goal.x + 0.5) * self.mGoalSprite.w, (goal.y + 0.5) * self.mGoalSprite.h,
                    self.mGoalSprite.x,
                    self.mGoalSprite.y,
                    self.mGoalSprite.w,
                    self.mGoalSprite.h)
            end
            for _, box in ipairs(self.mStage.boxes) do
                self.mSprite:draw((box.x - 1) * self.mBoxSprite.w, (box.y - 1) * self.mBoxSprite.h,
                    self.mBoxSprite.x,
                    self.mBoxSprite.y,
                    self.mBoxSprite.w,
                    self.mBoxSprite.h)
            end
        end

        obj.mSprite = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.png")
        obj.mSpriteConfig = (ParseXML(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.xml")):getData()
        obj.mRedPoint = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/red.png")
        obj.mX = 0
        obj.mY = 0
        obj.count = 0
        function obj:draw()
            -- self:drawSprite(self.count)
            self:drawBackground()
            self.mRedPoint:draw(self.mX, self.mY, 0, 0, 20, 20)
        end

        function obj:drawSprite(count)
            local id = count % #self.mSpriteConfig.children + 1
            local s = self.mSpriteConfig.children[id]
            local x = s.attributes.x
            local y = s.attributes.y
            local w = s.attributes.width
            local h = s.attributes.height
            local c = s.attributes.name
            self.mSprite:draw(0, 0, x, y, w, h)
            Framework:drawDebugString(20, 20, c)
        end

        obj.mGroundSprite = obj:getSprite("Ground_Dirt.png")
        obj.mWallSprite = obj:getSprite("Wall_Brown.png")
        obj.mGoalSprite = obj:getSprite("EndPoint_Yellow.png")
        obj.mBoxSprite = obj:getSprite("CrateDark_Beige.png")
        obj.mBoxOneGoalSprite = obj:getSprite("CrateDark_Beige.png")



        function obj:update()
            self.mX = self.mX + 1
            if self.mX > 300 then
                self.mY = self.mY + 1
                if self.mY > 300 then
                    self.mX = 0
                    self.mY = 0
                end
            end
            if Framework:isKeyTriggered(Enum.Keyboard.Space) then
                self.count = self.count + 1
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
