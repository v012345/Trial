xpcall(function()
    require "Math.Vector2"
    require "Math.Matrix22"
    local gImage = Image(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0
    local function rotate(out, input, offset, matrix)
        out:setSub(input, offset);
        matrix:multiply(out, out);
        out = out + offset;
    end

    ---@param out Vector2
    local function scale(out, input, offset, ratio)
        out:setMul(ratio, input);
        out = out + offset;
    end

    local function transform(out, input, scalingOffset, scalingRatio, rotationOffset, rotationMatrix)
        out:setMul(scalingRatio, input);
        out = out + scalingOffset;
        out = out - rotationOffset;
        rotationMatrix:multiply(out, out);
        out = out + rotationOffset;
    end
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
            local sine = math.sin(rotation * math.pi / 180)
            local cosine = math.cos(rotation * math.pi / 180)
            local rotationMatrix = Matrix22(cosine, -sine, sine, cosine)
            local rotationOffset = Vector2(ww / 2, wh / 2)
            local scalingRatio = Vector2(1.1 + math.sin(rotation * math.pi / 180),
                1.1 + math.cos(rotation * math.pi / 180));
            local scalingOffset = Vector2(ww / 2 - iw / 2 * scalingRatio.x, wh / 2 - ih / 2 * scalingRatio.y);
            local a, b, c = Vector2(), Vector2(), Vector2()
            transform(a, Vector2(0, 0), scalingOffset, scalingRatio, rotationOffset, rotationMatrix);
            transform(b, Vector2(iw, 0), scalingOffset, scalingRatio, rotationOffset, rotationMatrix);
            transform(c, Vector2(0, ih), scalingOffset, scalingRatio, rotationOffset, rotationMatrix);
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
