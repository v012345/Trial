xpcall(function()
    require "Object"
    require "Enum"
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    -- require "Director"
    -- Framework:setFrameRate(60)
    -- Director:update()
    local p = {
        "res/blue.png",
        "res/red.png",
        "res/green.png",
        "res/bg.png",
    }
    local images = {}
    for index, value in ipairs(p) do
        images[#images + 1] = Image(CMAKE_CURRENT_SOURCE_DIR .. value)
    end
    for index, value in ipairs(p) do
        images[#images + 1] = Image(CMAKE_CURRENT_SOURCE_DIR .. value)
    end
    for index, value in ipairs(p) do
        images[#images + 1] = Image(CMAKE_CURRENT_SOURCE_DIR .. value)
    end
    for index, value in ipairs(p) do
        images[#images + 1] = Image(CMAKE_CURRENT_SOURCE_DIR .. value)
    end
    for index, value in ipairs(p) do
        images[#images + 1] = Image(CMAKE_CURRENT_SOURCE_DIR .. value)
    end
    for index, value in ipairs(p) do
        images[#images + 1] = Image(CMAKE_CURRENT_SOURCE_DIR .. value)
    end
    for index, value in ipairs(p) do
        images[#images + 1] = Image(CMAKE_CURRENT_SOURCE_DIR .. value)
    end

    function MainLoop()
        xpcall(function()
            for index, value in ipairs(images) do
                value:draw()
            end
            -- Director:update()
            -- StringRenderer:draw(0, 0, "jifskjkl")
            Framework:drawDebugString(1, 1, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
