xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "MainLoop"
end, function(msg)
    print(msg)
end)
