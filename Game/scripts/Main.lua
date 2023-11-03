-- debug.sethook(function(a, b)
--     print(b, debug.getinfo(2).source)
-- end, "l")
xpcall(function()
    function IsA(A, B)
        if A == B then
            return true
        else
            local super = getmetatable(A)
            if super and type(super.__index) == "table" then
                return IsA(super, B)
            else
                return false
            end
        end
    end

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
