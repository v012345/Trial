xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    Game.PreviousTime = {}
    for i = 1, 10, 1 do
        Game.PreviousTime[i] = Framework.time();
    end
    Game.Counter = 0;
    require "Keyboard"
    require "MainLoop"
end, function(msg)
    print(msg)
end)
