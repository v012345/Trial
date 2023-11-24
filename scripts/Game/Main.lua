require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Game.Enum"
    require "Math.Vector2"
    require "Math.Matrix23"
    local gDepthTest = true;
    local gDepthWrite = true;
    local gSwapOrder = false;
    local gBlendMode = Enum.BlendMode.BLEND_LINEAR;
    local gZMove = -999.0
    local gXMove = -0.0
    -- Framework:setFrameRate(60)
    function MainLoop()
        xpcall(function()
            local p = {
                { 1500.0, 500.0,  1000.0 },
                { 500.0,  1500.0, 1000.0 },
                { 1500.0, 500.0,  2000.0 },
                { 500.0,  1500.0, 2000.0 },
            }
            for i = 1, 4 do
                p[i][1] = p[i][1] + gXMove
                p[i][3] = p[i][3] + gZMove
                p[i][1] = p[i][1] / p[i][3]
                p[i][2] = p[i][2] / p[i][3]
                p[i][3] = (1.0 / 9999.0) * p[i][3] - (1.0 / 9999.0)
            end

            Framework:drawTriangle3D(p[1], p[2], p[3]);
            Framework:drawTriangle3D(p[4], p[2], p[3]);
            if Framework:isKeyOn(Enum.Keyboard.S) then
                gZMove = gZMove - 1
            elseif Framework:isKeyOn(Enum.Keyboard.W) then
                gZMove = gZMove + 1
            end
            if Framework:isKeyOn(Enum.Keyboard.A) then
                gXMove = gXMove - 1
            elseif Framework:isKeyOn(Enum.Keyboard.D) then
                gXMove = gXMove + 1
            end
            Framework:drawDebugString(10, 0, "[w-s] Z MOVEMENT: " .. gZMove)
            Framework:drawDebugString(10, 1, "[a-d] X MOVEMENT: " .. gXMove)
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
