xpcall(function()
    require "Object"
    require "Enum"
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    ---@return Square
    Square = {}
    setmetatable(Square, {
        __call = function(self, file)
            ---@class Square:Object
            local obj = {}
            function obj:isA(what)
                return Square == what
            end

            obj.pImage = Image(file)

            function obj:set(x, y, halfSize)
                self.mX = x;
                self.mY = y;
                self.mHalfSize = halfSize;
            end

            ---@param a Square
            function obj:isIntersect(a)
                local la = self.mX - self.mHalfSize;
                local ra = self.mX + self.mHalfSize;
                local lb = a.mX - a.mHalfSize;
                local rb = a.mX + a.mHalfSize;
                if (la < rb) and (ra > lb) then
                    local ta = self.mY - self.mHalfSize;
                    local ba = self.mY + self.mHalfSize;
                    local tb = a.mY - a.mHalfSize;
                    local bb = a.mY + a.mHalfSize;
                    if (ta < bb) and (ba > tb) then
                        return true;
                    end
                end
                return false;
            end

            function obj:update()
                self.pImage:draw(self.mX - self.mHalfSize, self.mY - self.mHalfSize, 0, 0, 32, 32)
            end

            setmetatable(obj, { __index = Object() })
            return obj
        end
    })
    ---@type Square
    local gPlayer = Square(CMAKE_CURRENT_SOURCE_DIR .. "res/red.png")
    ---@type Square
    local gWall = Square(CMAKE_CURRENT_SOURCE_DIR .. "res/green.png")
    gPlayer:set(16, 16, 16);
    gWall:set(160, 120, 16);
    Framework:setFrameRate(60);
    local bg = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/bg.png")
    function MainLoop()
        xpcall(function()
            local dx = 0;
            local dy = 0;
            if Framework:isKeyOn(Enum.Keyboard.A) then
                dx = dx - 1
            elseif Framework:isKeyOn(Enum.Keyboard.S) then
                dy = dy + 1
            elseif Framework:isKeyOn(Enum.Keyboard.D) then
                dx = dx + 1
            elseif Framework:isKeyOn(Enum.Keyboard.W) then
                dy = dy - 1
            end
            gPlayer.mX = gPlayer.mX + dx;
            gPlayer.mY = gPlayer.mY + dy;
            if gPlayer:isIntersect(gWall) then
                gPlayer.pImage = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/blue.png")
                gPlayer.mX = gPlayer.mX - dx;
                gPlayer.mY = gPlayer.mY - dy;
            else
                gPlayer.pImage = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/red.png")
            end



            bg:draw()
            gPlayer:update()
            gWall:update()
            print(Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
