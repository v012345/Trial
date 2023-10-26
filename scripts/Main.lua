xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "init"
    require "Director"
    -- Framework:init()
    ---@type GameReady
    local director = Director()
    function MainLoop()
        xpcall(function()
            director:update()
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
