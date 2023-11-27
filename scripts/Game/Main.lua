require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Game.Enum"
    require "Math.Vector2"
    require "Math.Matrix23"
    local gZMove = 0;
    local gXMove = 0;
    local gYMove = 0;
    -- Framework:setFrameRate(60)
    function MainLoop()
        xpcall(function()
            local p = {
                { 1500.0, 500.0,  6000.0 },
                { 500.0,  1500.0, 6000.0 },
                { 1500.0, 500.0,  7000.0 },
                { 500.0,  1500.0, 7000.0 },
            }
            for i = 1, 4 do
                p[i][1] = p[i][1] + gXMove
                p[i][2] = p[i][2] + gYMove
                p[i][3] = p[i][3] + gZMove
                p[i][1] = p[i][1] / p[i][3] * 640.0;
                p[i][2] = p[i][2] / p[i][3] * 640.0;
                p[i][1] = p[i][1] + 320
                p[i][2] = p[i][2] + 240
                p[i][3] = (1.0 / 9999.0) * p[i][3] - (1.0 / 9999.0)
            end

            Framework:drawTriangle3D(p[1], p[2], p[3]);
            Framework:drawTriangle3D(p[4], p[2], p[3], 0, 0, 0, 0xffff0000, 0xffff0000, 0xffff0000);
            if Framework:isKeyOn(Enum.Keyboard.Q) then
                gZMove = gZMove - 1
            elseif Framework:isKeyOn(Enum.Keyboard.E) then
                gZMove = gZMove + 1
            end
            if Framework:isKeyOn(Enum.Keyboard.S) then
                gYMove = gYMove - 1
            elseif Framework:isKeyOn(Enum.Keyboard.W) then
                gYMove = gYMove + 1
            end
            if Framework:isKeyOn(Enum.Keyboard.A) then
                gXMove = gXMove - 1
            elseif Framework:isKeyOn(Enum.Keyboard.D) then
                gXMove = gXMove + 1
            end
            Framework:drawDebugString(10, 0, "[w-s] Y MOVEMENT: " .. gYMove)
            Framework:drawDebugString(10, 1, "[a-d] X MOVEMENT: " .. gXMove)
            Framework:drawDebugString(10, 2, "[q-e] Z MOVEMENT: " .. gZMove)
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
