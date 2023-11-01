xpcall(function()
    require "Object"
    require "Enum"
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    -- require "Director"
    Framework:setFrameRate(60)
    -- Director:update()

    function MainLoop()
        xpcall(function()
            -- Director:update()
            StringRenderer:draw(0,0,"jifskjkl")
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
