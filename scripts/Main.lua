xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    -- print(Image)
    -- local x = Image(CMAKE_SOURCE_DIR .. "res/dummy.png")
    -- x:draw()
    -- require "init"
    -- require "Image"
    -- require "ScenceBase"
    -- require "Director"
    -- Framework:init()
    -- local t1 = Framework:time()
    -- ---@type Director
    -- local director = Director()
    function MainLoop()
        xpcall(function()
            -- Framework:fixFPS()
            -- director:update()
            -- Framework:drawDebugString(70, 1, "FPS:" .. tostring(math.ceil(1000 / (Framework:time() - t1))))
            -- t1 = Framework:time()
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
