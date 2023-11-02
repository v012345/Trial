xpcall(function()
    require "Object"
    require "Enum"
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    Framework:setFrameRate(60)
    require "DataStage"
    require "Director"
    function MainLoop()
        xpcall(function()
            Director:update()
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
