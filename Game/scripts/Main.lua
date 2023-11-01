xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    -- print(Image)
    local x = Image("D:\\Trial\\res\\translucent.png")
    print(x)
    print(x:height())
    print(x:width())
    x:draw()
    print(Framework:time())
    Framework:sleep(1000);
    print(Framework:time())
    -- require "init"
    -- require "Image"
    -- require "ScenceBase"
    -- require "Director"
    -- Framework:init()
    -- local t1 = Framework:time()
    -- ---@type Director
    -- local director = Director()
    -- local s = {}
    -- for i = 1, 100, 1 do
    --     s[#s + 1] = Framework:time()
    -- end
    Framework:setFrameRate(60)

    function MainLoop()
        xpcall(function()
            if Framework:isKeyOn(97) then
                print(Framework:frameRate())
            end
            -- Framework:fixFPS()
            -- director:update()
            -- Framework:drawDebugString(70, 1,
            -- print("FPS:" .. tostring(math.ceil(100000 / (Framework:time() - s[1]))))
            -- for i = 1, 99, 1 do
            --     s[i] = s[i + 1]
            -- end
            -- s[100] = Framework:time()
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
