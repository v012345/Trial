function MainLoop()
    xpcall(function()
        local currentTime = Framework.time()
        -- 这里有溢出问题
        while currentTime - Game.PreviousTime[10] < 16 do
            currentTime = Framework.time()
            Framework.sleep(1);
        end
        for i = 1, 9, 1 do
            Game.PreviousTime[i] = Game.PreviousTime[i + 1];
        end
        Game.PreviousTime[10] = currentTime
        -- local frameInterval10 = Game.PreviousTime[10] - Game.PreviousTime[1];
        -- if Game.iCounter % 60 == 0 then
        --     print(Game.iCounter // 60, "frameInterval:", frameInterval10 / 10)
        --     print(Game.iCounter // 60, " FrameRate:", 10 * 1000 / frameInterval10)
        -- end
        -- Game:dealInput()
        -- Game:update()
        -- Game.iCounter = Game.iCounter + 1
        SceneManager:update()
        ViewManager:update()
    end, function(msg)
        print(msg)
    end)
end
