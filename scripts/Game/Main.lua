xpcall(function()
    require "Game.Enum"
    require "Math.Vector2"
    require "Math.Matrix23"
    local gImage = Image(CMAKE_SOURCE_DIR .. "res/background.png")
    local gImageWidth = gImage:width();
    local gImageHeight = gImage:height();
    ---@diagnostic disable-next-line
    gImage = nil
    local gTexture = Framework:createTexture(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0;
    local gScaleFirst = true;
    Framework:setFrameRate(60)
    function MainLoop()
        xpcall(function()
            Framework:drawDebugString(0, 0, "press SPACE to swap rotation and scaling");
            -- Framework:drawDebugString(0, 1, "current blend mode of A : " .. a);
            -- Framework:drawDebugString(0, 2, "current blend mode of D : " .. b);
            Framework:setTexture(gTexture);
            local rotation = gCount
            local scale = Vector2(
                math.sin(rotation * math.pi / 180) + 0.5,
                math.cos(rotation * math.pi / 180) + 0.5
            )
            local m = Matrix23();
            m:setTranslation(Vector2(gImageWidth / 2, gImageHeight / 2));
            if gScaleFirst then
                m:rotate(rotation);
                m:scale(scale);
            else
                m:scale(scale);
                m:rotate(rotation);
            end
            m:translate(Vector2(-gImageWidth / 2, -gImageHeight / 2))
            local p0 = Vector2(0.0, 0.0);
            local p1 = Vector2(100.0, 0.0);
            local p2 = Vector2(0.0, 100.0);
            local p3 = Vector2(100.0, 100.0);
            local t0 = { 0.0, 0.0 };
            local t1 = { 1.0, 0.0 };
            local t2 = { 0.0, 1.0 };
            local t3 = { 1.0, 1.0 };
            m:multiply(p0, p0);
            m:multiply(p1, p1);
            m:multiply(p2, p2);
            m:multiply(p3, p3);
            Framework:drawTriangle2D({ p0.x, p0.y }, { p1.x, p1.y }, { p2.x, p2.y }, t0, t1, t2);
            Framework:drawTriangle2D({ p3.x, p3.y }, { p1.x, p1.y }, { p2.x, p2.y }, t3, t1, t2);
            gCount = gCount + 1
            if Framework:isKeyTriggered(Enum.Keyboard.Space) then
                gScaleFirst = not gScaleFirst;
            end
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
