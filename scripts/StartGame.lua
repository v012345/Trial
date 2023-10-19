xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "enum"
    local map = require "ReadStageData"
    for index, value in ipairs(map) do
        for index, value in ipairs(value) do
            io.write(value, "\t")
        end
        print()
    end
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
    function Game.drawImage(IMAGE, atX, atY, isBG)
        for y, row in ipairs(IMAGE) do
            for x, argb in ipairs(row) do
                if isBG then
                    Impl:vram(atX + x - 1, atY + y - 1, argb)
                else
                    local srcA = (argb & 0xff000000) >> 24
                    local srcR = argb & 0xff0000;
                    local srcG = argb & 0x00ff00;
                    local srcB = argb & 0x0000ff;
                    local dst = Impl:colorAt(atX + x - 1, atY + y - 1)
                    local dstR = dst & 0xff0000;
                    local dstG = dst & 0x00ff00;
                    local dstB = dst & 0x0000ff;
                    local r = (srcR - dstR) * srcA // 255 + dstR;
                    local g = (srcG - dstG) * srcA // 255 + dstG;
                    local b = (srcB - dstB) * srcA // 255 + dstB;
                    -- 如果大于255则设置为255
                    r = (r > 0xff0000) and 0xff0000 or r;
                    g = (g > 0x00ff00) and 0x00ff00 or g;
                    b = (b > 0x0000ff) and 0x0000ff or b;
                    dst = (r & 0xff0000) | (g & 0x00ff00) | b;
                    Impl:vram(atX + x - 1, atY + y - 1, dst)
                end
            end
        end
    end

    Game.entity = require "ConertStagesToImage"

    Game.stage = require "Stages"[4]
    Game.player = { x = 1, y = 1 }
    Game.box = {}
    Game.isMoving = false
    Game.step = 0
    Game.dx = 0
    Game.dy = 0
    local background = {}
    for y, data in ipairs(Game.stage) do
        local row = {}
        for x, obj in ipairs(data) do
            if obj == 0 then
                row[#row + 1] = 0
            elseif obj == 1 then
                Game.player.x = x
                Game.player.y = y
                row[#row + 1] = 0
            elseif obj == 6 then
                row[#row + 1] = 6
            elseif obj == 5 then
                row[#row + 1] = 5
            elseif obj == 3 then
                Game.box[#Game.box + 1] = {
                    x = x,
                    y = y
                }
                row[#row + 1] = 0
            else
                row[#row + 1] = 7
            end
        end
        background[#background + 1] = row
    end
    Game.background = background
    require "Keyboard"
    require "MainLoop"
end, function(msg)
    print(msg)
end)
