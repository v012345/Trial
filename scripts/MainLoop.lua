function MainLoop()
    xpcall(function()
        local currentTime = Framework.time()
        -- 这里有溢出问题
        while currentTime - Framework.PreviousTime[10] < 16 do
            currentTime = Framework.time()
            Framework.sleep(1);
        end
        for i = 1, 9, 1 do
            Framework.PreviousTime[i] = Framework.PreviousTime[i + 1];
        end
        Framework.PreviousTime[10] = currentTime
        SceneManager:update()
        ViewManager:update()
        Framework:showFPS()
    end, function(msg)
        print(msg)
    end)
end
