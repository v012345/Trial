xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "RenderEntity"
    require "Engine"
    require "Enum"
    require "Game"
    Game:init()
    Game:readStageData(CMAKE_SOURCE_DIR .. "example/stageData.txt")
    Game:dumpBackground()
    Game:loadEntities(map)
    Game:dumpEntities()
    Game:dumpMapWithEmoji()
    Game:loadRenderImage(CMAKE_SOURCE_DIR .. "res/nimotsuKunImageAlpha.png")
    Game:drawBackground()
    require "Keyboard"
    require "MainLoop"
end, function(msg)
    print(msg)
end)
