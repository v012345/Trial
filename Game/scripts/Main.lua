xpcall(function()
    local gImage = Image(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0
    local function rotate(x, y, offsetX, offsetY, rotation)
        local xf = x + 0.5;
        local yf = y + 0.5;
        xf = xf - offsetX
        yf = yf - offsetY
        local r = math.sqrt(xf * xf + yf * yf)
        local angle = math.atan(yf, xf) * 180 / math.pi
        angle = (angle + rotation) * math.pi / 180;
        local sine = math.sin(angle);
        local cosine = math.cos(angle);
        xf = r * cosine;
        yf = r * sine;
        xf = xf + offsetX
        yf = yf + offsetY
        return math.floor(xf), math.floor(yf)
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

            local offsetX = iw / 2;
            local offsetY = ih / 2;
            local rotation = gCount
            for y = 0, ih - 1 do
                for x = 1, iw - 1 do
                    local rx, ry = rotate(x, y, offsetX, offsetY, rotation);
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
