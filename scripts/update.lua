xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    function MainLoop()
    end
end, function(msg)
    print(msg)
end)
