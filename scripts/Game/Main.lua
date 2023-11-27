require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Game.Enum"
    require "Math.Vector2"
    require "Math.Matrix23"
    local gZMove = 5000.0;
    -- Framework:setFrameRate(60)
    local texture = Framework:createTexture(CMAKE_SOURCE_DIR .. "Example/grid.tga")
    function MainLoop()
        xpcall(function()
            local p = {
                { -500.0, 300.0, -1000.0 },
                { -500.0, 300.0, 1000.0 },
                { 500.0,  300.0, -1000.0 },
                { 500.0,  300.0, 1000.0 },
            }
            local uv = {
                { 0.0, 0.0 },
                { 1.0, 0.0 },
                { 0.0, 1.0 },
                { 1.0, 1.0 },
            }
            for i = 1, 4 do
                p[i][3] = p[i][3] + gZMove
                p[i][1] = p[i][1] / p[i][3] * 640.0;
                p[i][2] = p[i][2] / p[i][3] * 640.0;
                p[i][1] = p[i][1] + 320
                p[i][2] = p[i][2] + 240
                p[i][3] = (1.0 / 9999.0) * p[i][3] - (1.0 / 9999.0)
            end
            Framework:setTexture(texture)
            Framework:drawTriangle3D(p[1], p[2], p[3], uv[1], uv[2], uv[3], 0xffff0000, 0xffff0000, 0xffff0000);
            Framework:drawTriangle3D(p[4], p[2], p[3], uv[4], uv[2], uv[3], 0xff00ff00, 0xff00ff00, 0xff00ff00);
            if Framework:isKeyOn(Enum.Keyboard.W) then
                gZMove = gZMove + 25
            elseif Framework:isKeyOn(Enum.Keyboard.S) then
                gZMove = gZMove - 25
            end

            Framework:drawDebugString(10, 0, "[w-s] Z MOVEMENT: " .. gZMove)
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
