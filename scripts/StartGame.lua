xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "enum"
    require "Game"
    local map = require "ReadStageData"
    Game:loadBackground(map)
    Game:dumpBackground()
    Game:loadEntities(map)
    Game:dumpEntities()
    Game:dumpMapWithEmoji()
    Game:loadRenderImage(CMAKE_SOURCE_DIR .. "res/nimotsuKunImageAlpha.png")
    function MainLoop()

    end

    do
        return
    end
    Game.PreviousTime = {}
    for i = 1, 10, 1 do
        Game.PreviousTime[i] = Framework.time();
    end
    Game.Counter = 0;
    

    Game.entity = require "ConertStagesToImage"

    Game.stage = require "Stages"[4]
    Game.player = { x = 1, y = 1 }
    Game.box = {}
    Game.isMoving = false
    Game.step = 0
    Game.dx = 0
    Game.dy = 0

    require "Keyboard"
    require "MainLoop"
end, function(msg)
    print(msg)
end)
