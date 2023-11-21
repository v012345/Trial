xpcall(function()
    require "Math.Vector2"
    require "Math.Matrix22"
    require "Math.Matrix23"
    local gImage = Image(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0
    print(math.atan(1, 1) * 180 / math.pi)
    print(math.atan(1, -1) * 180 / math.pi)
    print(math.atan(-1, -1) * 180 / math.pi)
    print(math.atan(-1, 1) * 180 / math.pi)
    function MainLoop()
        xpcall(function()
            local ww = Framework:width()
            local wh = Framework:height()
            for y = 0, wh - 1 do
                for x = 0, ww - 1 do
                    Framework:setVideoMemory(x, y, 0)
                end
            end
            local iw = gImage:width();
            local ih = gImage:height();
            local rotation = gCount
            local translationMatrix1 = Matrix23()
            translationMatrix1:setTranslation(Vector2(-iw / 2, -ih / 2));
            local scalingMatrix = Matrix23()
            local scale = Vector2(math.sin(rotation * math.pi / 180) * 2.0 + 1.0,
                math.cos(rotation * math.pi / 180) * 2.0 + 1.0);
            scalingMatrix:setScaling(Vector2(scale.x, scale.y));
            local rotationMatrix = Matrix23()
            rotationMatrix:setRotation(rotation);
            local translationMatrix2 = Matrix23()
            translationMatrix2:setTranslation(Vector2(iw / 2, ih / 2));
            local transform = Matrix23();
            transform = translationMatrix2;
            transform = transform * rotationMatrix;
            transform = transform * scalingMatrix;
            transform = transform * translationMatrix1;
            local a, b, c = Vector2(), Vector2(), Vector2()
            transform:multiply(a, Vector2(0, 0));
            transform:multiply(b, Vector2(iw, 0));
            transform:multiply(c, Vector2(0, ih));
            local ab, ac = Vector2(), Vector2()
            ab:setSub(b, a);
            ac:setSub(c, a);
            local rcpWidth = 1.0 / iw;
            local rcpHeight = 1.0 / ih;
            for y = 0, ih - 1 do
                local v = (y + 0.5) * rcpHeight
                for x = 1, iw - 1 do
                    local u = (x + 0.5) * rcpWidth
                    local p = Vector2();
                    p:setInterporation(a, ab, ac, u, v);
                    local rx = math.floor(p.x);
                    local ry = math.floor(p.y);
                    if rx >= 0 and rx < ww and ry >= 0 and ry < wh then
                        Framework:setVideoMemory(rx, ry, gImage:pixel(x, y))
                    end
                end
            end
            gCount = gCount + 1
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
