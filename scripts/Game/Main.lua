require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Game.Enum"
    require "Math.Vector2"
    require "Math.Matrix23"
    local gDepthTest = true;
    Framework:setFrameRate(60)
    function MainLoop()
        xpcall(function()
            Framework:enableDepthTest(gDepthTest)
            local p0 = { 200.0, 100.0, 1.0 }
            local p1 = { 400.0, 100.0, 1.0 }
            local p2 = { 300.0, 400.0, -0.5 }
            local c = 0xffff0000
            Framework:drawTriangle3D(p0, p1, p2, 0, 0, 0, c, c, c);
            p0 = { 200.0, 400.0, 1.5 }
            p1 = { 400.0, 400.0, 1.0 }
            p2 = { 300.0, 100.0, 0.0 }
            c = 0xff00ff00
            Framework:drawTriangle3D(p0, p1, p2, 0, 0, 0, c, c, c);
            if Framework:isKeyTriggered(Enum.Keyboard.Space) then
                gDepthTest = not gDepthTest
            end
            if gDepthTest then
                Framework:drawDebugString(0, 0, "Depth Test : On (press space)")
            else
                Framework:drawDebugString(0, 0, "Depth Test : Off (press space)")
            end
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
