xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "init"
    ViewManager:init()
    SceneManager:load("Title")
    local sp = ReadPngFile(CMAKE_SOURCE_DIR .. "res/Link.png")
    print()
    local x = {}
    for n = 1, 10, 1 do
        local y = {}
        for i = 520, 623 do
            local r = {}
            for j = (n - 1) * 95 + 1, n * 95, 1 do
                r[#r + 1] = sp[i][j]
                -- io.write(r[j], "\t")
            end
            y[#y + 1] = r
            -- io.write("\n")
        end
        x[#x + 1] = y
    end

    local sprite = {
        count = 1,
        x = x
    }
    -- Game:dumpBackground()
    -- Game:dumpMapWithEmoji()
    function MainLoop()
        xpcall(function()
            local currentTime = Framework.time()
            -- 这里有溢出问题
            while currentTime - Framework.PreviousTime[10] < 33 do
                currentTime = Framework.time()
                Framework.sleep(1);
            end
            for i = 1, 9, 1 do
                Framework.PreviousTime[i] = Framework.PreviousTime[i + 1];
            end
            Framework.PreviousTime[10] = currentTime
            -- SceneManager:update()
            -- ViewManager:update()
            Impl:clear()
            sprite.count = (sprite.count) % 10 + 1
            Framework:draw(sprite.x[sprite.count], 0, 0, false)
            Framework:showFPS()
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
