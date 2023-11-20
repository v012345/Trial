xpcall(function()
    require "Math.Vector2"
    require "Math.Matrix22"
    local gImage = Image(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0
    local function rotate(x, y, offset, matrix)
        local p = Vector2(x, y)
        local tmpOffset = Vector2(0.5, 0.5)
        p = p + tmpOffset
        p = p - offset
        local r = Vector2()
        matrix:multiply(r, p)
        r = r + offset
        return math.floor(r.x), math.floor(r.y)
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
            local offset = Vector2()
            offset.x = iw / 2;
            offset.y = ih / 2;
            local rotation = gCount
            local sine = math.sin(rotation * math.pi / 180)
            local cosine = math.cos(rotation * math.pi / 180)
            local matrix = Matrix22(cosine, -sine, sine, cosine);
            for y = 0, ih - 1 do
                for x = 1, iw - 1 do
                    local rx, ry = rotate(x, y, offset, matrix);
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
