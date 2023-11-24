require("LuaPanda").start("127.0.0.1", 8818);
xpcall(function()
    require "Game.Enum"
    require "Math.Vector2"
    require "Math.Matrix23"
    local gDepthTest = true;
    local gDepthWrite = true;
    local gSwapOrder = false;
    local gBlendMode = Enum.BlendMode.BLEND_LINEAR;

    Framework:setFrameRate(60)
    function MainLoop()
        xpcall(function()
            Framework:enableDepthTest(gDepthTest)
            Framework:enableDepthWrite(gDepthWrite);
            Framework:setBlendMode(gBlendMode);
            local p0 = { 200.0, 100.0, 1.0 }
            local p1 = { 400.0, 100.0, 1.0 }
            local p2 = { 300.0, 400.0, -0.5 }
            local c1 = 0x80ff0000
            local p3 = { 200.0, 400.0, 1.5 }
            local p4 = { 400.0, 400.0, 1.0 }
            local p5 = { 300.0, 100.0, 0.0 }
            local c2 = 0x8000ff00
            if gSwapOrder then
                Framework:drawTriangle3D(p3, p4, p5, 0, 0, 0, c2, c2, c2);
                Framework:drawTriangle3D(p0, p1, p2, 0, 0, 0, c1, c1, c1);
            else
                Framework:drawTriangle3D(p0, p1, p2, 0, 0, 0, c1, c1, c1);
                Framework:drawTriangle3D(p3, p4, p5, 0, 0, 0, c2, c2, c2);
            end


            if Framework:isKeyTriggered(Enum.Keyboard.Space) then
                gDepthTest = not gDepthTest
            end
            if gDepthTest then
                Framework:drawDebugString(0, 0, "Depth Test : On (press space)")
            else
                Framework:drawDebugString(0, 0, "Depth Test : Off (press space)")
            end

            if Framework:isKeyTriggered(Enum.Keyboard.D) then
                gDepthWrite = not gDepthWrite
            end
            if gDepthWrite then
                Framework:drawDebugString(0, 2, "[d] Depth Write : On");
            else
                Framework:drawDebugString(0, 2, "[d] Depth Write : Off");
            end
            if Framework:isKeyTriggered(Enum.Keyboard.S) then
                gSwapOrder = not gSwapOrder
            end
            if gSwapOrder then
                Framework:drawDebugString(0, 1, "[s] draw Green first");
            else
                Framework:drawDebugString(0, 1, "[s] draw Red first");
            end

            if Framework:isKeyTriggered(Enum.Keyboard.A) then
                if gBlendMode == Enum.BlendMode.BLEND_LINEAR then
                    gBlendMode = Enum.BlendMode.BLEND_ADDITIVE
                else
                    gBlendMode = Enum.BlendMode.BLEND_LINEAR
                end
            end
            if gBlendMode == Enum.BlendMode.BLEND_LINEAR then
                Framework:drawDebugString(0, 3, "[a] Linear Blend");
            else
                Framework:drawDebugString(0, 3, "[a] Additive Blend");
            end

            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
