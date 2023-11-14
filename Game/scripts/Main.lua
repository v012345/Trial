-- debug.sethook(function(a, b)
--     print(b, debug.getinfo(2).source)
-- end, "l")
xpcall(function()
    function IsA(A, B)
        if A == B then
            return true
        else
            local super = getmetatable(A)
            if super and type(super.__index) == "table" then
                return IsA(super, B)
            else
                return false
            end
        end
    end

    local lfs = require "lfs"
    -- require "Object"
    -- require "Enum"
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    Framework:setFrameRate(60)
    -- require "DataStage"
    -- require "Director"
    local m = {}
    local function getFiles(folder)
        for entry in lfs.dir(folder) do
            if entry ~= "." and entry ~= ".." then
                local filePath = folder .. "/" .. entry
                local fileAttributes = lfs.attributes(filePath)
                if fileAttributes.mode == "file" then
                    if string.match(string.lower(filePath), "^.+%.csd$") then
                        m[entry] = filePath
                    end
                end
            end
        end
        return m
    end
    -- print(package.path)
    -- require "Tools.ParseCSV"
    -- local x = ParseCSV("C:\\Users\\Meteor\\Downloads\\trans-client.csv")
    -- local x = ParseCSV("C:\\Users\\Meteor\\Desktop\\New Microsoft Excel Worksheet (2).csv")
    -- x:writeTo("tst.csv")
    -- local x = ParseCSV("D:\\Closers.cocos\\resource\\ui\\branches\\dzogame_sea\\zhcn\\trans.csv")
    -- local x = ParseCSV("D:\\Trial\\Game\\scripts\\1.txt")
    -- getFiles("D:\\Closers.cocos\\resource\\ui\\branches\\dzogame_sea\\zhcn\\cocosstudio\\ui")
    -- local x = {}
    -- require "Tools.ParseXML"
    -- for index, value in pairs(m) do
    --     print(index)
    --     x[index] = ParseXML(value)
    -- end
    -- for index, value in pairs(x) do
    --     print(index)
    --     value:writeTo("csd/" .. index)
    -- end
    -- local x = ParseXML(
    --     "D:\\Closers.cocos\\resource\\ui\\branches\\dzogame_sea\\zhcn\\cocosstudio\\ui\\UiCommonDialogSub2.csd")
    -- x:writeTo("UiCommonDialogSub2.xml")
    -- print(utf8)
    -- for key, value in pairs(utf8) do
    --     print(key, value)
    -- end
    -- 主循环协程
    local button = {
        normal = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/button-blue.png"),
        pressed = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/button-gray.png"),
        status = "normal"
    }

    local bg = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/console.png")


    local render = coroutine.create(function()
        while true do
            -- 执行主循环逻辑
            bg:draw()
            button[button.status]:draw()
            coroutine.yield()
        end
    end)

    -- 费时操作协程
    local logic  = coroutine.create(function()
        while true do
            local mouse = Framework:mouse()
            if mouse.left then
                button.status = "pressed"
            else
                button.status = "normal"
            end
            coroutine.yield()
        end
    end)
    function MainLoop()
        xpcall(function()
            -- Director:update()
            coroutine.resume(render)
            coroutine.resume(logic)

            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
            -- local mouse = Framework:mouse()
            -- Framework:drawDebugString(0, 20, mouse.x)
            -- Framework:drawDebugString(0, 21, mouse.y)
            -- Framework:drawDebugString(0, 23, tostring(mouse.left))
            -- Framework:drawDebugString(0, 22, tostring(mouse.right))
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
