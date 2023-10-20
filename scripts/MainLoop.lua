local function dealInput()
    if Framework.isKeyOn(Keyboard.A) then
        print("A")
        Game.dx = -1
        Game.isMoving = true
    end
    if Framework.isKeyOn(Keyboard.S) then
        print("S")
        Game.dy = 1
        Game.isMoving = true
    end
    if Framework.isKeyOn(Keyboard.D) then
        print("D")
        Game.dx = 1
        Game.isMoving = true
    end
    if Framework.isKeyOn(Keyboard.W) then
        print("W")
        Game.dy = -1
        Game.isMoving = true
    end
    if Framework.isKeyOn(Keyboard.Q) then
        print("Q")
    end
    if Framework.isKeyOn(Keyboard.R) then
        print("R")
    end
end

local function drawBackground()
    for y, row in ipairs(Game.background) do
        for x, obj in ipairs(row) do
            if obj == 6 then
                Game.drawImage(Game.entity.wall, (x - 1) * 32, (y - 1) * 32, true)
            end
            if obj == 5 then
                Game.drawImage(Game.entity.goal, (x - 1) * 32, (y - 1) * 32, true)
            end
            if obj == 0 then
                Game.drawImage(Game.entity.floor, (x - 1) * 32, (y - 1) * 32, true)
            end
        end
    end
end

local function drawForeground()
    for _, box in ipairs(Game.box) do
        Game.drawImage(Game.entity.box, (box.x - 1) * 32, (box.y - 1) * 32, false)
    end
    local x = (Game.player.x - 1) * 32 + (Game.dx * Game.step)
    local y = (Game.player.y - 1) * 32 + (Game.dy * Game.step)
    if G then
        
    end
    Game.drawImage(Game.entity.worker, (Game.player.x - 1) * 32 + (Game.dx * Game.step),
        (Game.player.y - 1) * 32 + (Game.dy * Game.step),
        false)
end
function MainLoop()
    xpcall(function()
        local currentTime = Framework.time()
        while currentTime - Game.PreviousTime[10] < 16 do
            currentTime = Framework.time()
            Framework.sleep(1);
        end
        for i = 1, 9, 1 do
            Game.PreviousTime[i] = Game.PreviousTime[i + 1];
        end
        Game.PreviousTime[10] = currentTime
        local frameInterval10 = Game.PreviousTime[10] - Game.PreviousTime[1];
        -- if Game.Counter % 60 == 0 then
        --     print(Game.Counter, "frameInterval:", frameInterval10 / 10)
        --     print(Game.Counter, " FrameRate:", 10 * 1000 / frameInterval10)
        -- end
        if not Game.isMoving then
            dealInput()
        end
        drawBackground()
        drawForeground()
        Game.Counter = Game.Counter + 1
    end, function(msg)
        print(msg)
    end)
end