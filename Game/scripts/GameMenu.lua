GameMenu = {}

setmetatable(GameMenu, {
    __call = function(self, callback)
        ---@class GameMenu
        local obj = {}
        function obj:isA(what)
            if GameMenu == what then
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

        obj.mImage = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/console.png")
        obj.mCursorPosistion = 1
        obj.mCallback = callback

        function obj:update()
            -- local next = self
            -- if Framework:isKeyTriggered(Enum.Keyboard.W) then
            --     self.mCursorPosistion = self.mCursorPosistion - 1
            --     if self.mCursorPosistion < 1 then
            --         self.mCursorPosistion = 2
            --     end
            -- elseif Framework:isKeyTriggered(Enum.Keyboard.S) then
            --     self.mCursorPosistion = self.mCursorPosistion + 1
            --     if self.mCursorPosistion > 2 then
            --         self.mCursorPosistion = 1
            --     end
            -- elseif Framework:isKeyTriggered(Enum.Keyboard.Space) then
            --     if self.mCursorPosistion == 1 then
            --         next = GamePlay()
            --     elseif self.mCursorPosistion == 2 then
            --         next = Title()
            --     end
            -- end
            if Framework:isKeyTriggered(Enum.Keyboard.Space) then
                if type(self.mCallback) == "function" then
                    self.mCallback()
                end
                return
            end
            self.mImage:draw(0, 0, 0, 0, 300, 50)

            Framework:drawDebugString(1, 1, "[pause]");
            Framework:drawDebugString(2, 3, "game continue");
            Framework:drawDebugString(2, 4, "return title");
            -- Framework:drawDebugString(1, self.mCursorPosistion + 2, ">");
            -- return next
        end

        setmetatable(obj, {
            __index = Object(),
            __tostring = function() return "GameMenu" end
        })
        return obj
    end
})
