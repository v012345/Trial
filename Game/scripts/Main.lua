xpcall(function()
    local gImage = Image(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0
    local function rotate(x, y, offsetX, offsetY, sine, cosine)
        local xf = x + 0.5;
        local yf = y + 0.5;
        xf = xf - offsetX
        yf = yf - offsetY

        local ryf = yf * cosine + xf * sine;
        local rxf = xf * cosine - yf * sine;

        ryf = ryf + offsetX
        rxf = rxf + offsetY
        return math.floor(rxf), math.floor(ryf)
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
            local sine = math.sin(rotation * math.pi / 180)
            local cosine = math.cos(rotation * math.pi / 180)
            for y = 0, ih - 1 do
                for x = 1, iw - 1 do
                    local rx, ry = rotate(x, y, offsetX, offsetY, sine, cosine);
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
