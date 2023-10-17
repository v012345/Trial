xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    -- require "NimotsuKun"
    -- require "RemoveBOM"
    require "png"
    print(package.path)
    local t = os.clock()
    function MainLoop()
        Impl:vram(math.random(0, 250), math.random(0, 500), math.random(0, 0xFFFFFF))
        local tt = os.clock()
        print(tt - t)
        t = tt
    end
end, function(msg)
    print(msg)
end)
