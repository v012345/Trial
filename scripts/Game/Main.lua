xpcall(function()
    require "Game.Enum"
    require "Math.Vector2"
    require "Math.Matrix23"
    local gCount = 0;
    Framework:setFrameRate(60)
    function MainLoop()
        xpcall(function()
            local p0 = { 100.0, 100.0, 0.0 };
            local p1 = { 400.0, 200.0, 0.4 };
            local p2 = { 200.0, 400.0, 0.8 };

            local zOffset = (gCount % 200) * 0.01 - 1.0;

            p0[3] = p0[3] + zOffset;
            p1[3] = p1[3] + zOffset;
            p2[3] = p2[3] + zOffset;
            Framework:drawTriangle3D(p0, p1, p2);
            gCount = gCount + 1
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
