function MainLoop()
    xpcall(function()
        local currentTime = Framework.time()
        while currentTime - Game.PreviousTime[10] < 16 do
            currentTime = Framework.time()
            Framework.sleep(1);
        end
        for i = 1, 9, 1 do
            Game.PreviousTime[i] = Game.PreviousTime[i + 1];
        end
        Game.PreviousTime[10] = currentTime
        local frameInterval10 = Game.PreviousTime[10] - Game.PreviousTime[1];
        -- if Game.Counter % 60 == 0 then
        --     print(Game.Counter, "frameInterval:", frameInterval10 / 10)
        --     print(Game.Counter, " FrameRate:", 10 * 1000 / frameInterval10)
        -- end
        if Framework.isKeyOn(Keyboard.A) then
            print(Game.Counter, "frameInterval:", frameInterval10 / 10)
            print(Game.Counter, " FrameRate:", 10 * 1000 / frameInterval10)
        end
        Game.Counter = Game.Counter + 1
    end, function(msg)
        print(msg)
    end)
end
