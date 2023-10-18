xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    local i = 1
    function MainLoop()
        Framework.sleep(20)
        print(i)
        i = i + 1
    end
end, function(msg)
    print(msg)
end)
