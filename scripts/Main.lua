xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "init"
    Framework:init()
    local BlueManSprite = Sprite:new(require("Config.BlueMan"))
    local BlueManEntity = Entity:new({
        Sprite = BlueManSprite
    })
    -- local Link = require "Config.Boss"
    -- ViewManager:init()
    -- SceneManager:load("Title")
    -- local x = SpriteManager:create(Link)
    function MainLoop()
        xpcall(function()
            Framework:fixFPS()
            Impl:clear()
            BlueManEntity:update()
            -- Impl:clear()
            -- x:update()
            -- if Framework.isKeyOn(Keyboard.A) then
            --     x:move(Direction.Left)
            -- end
            -- if Framework.isKeyOn(Keyboard.S) then
            --     x:move(Direction.Down)
            -- end
            -- if Framework.isKeyOn(Keyboard.D) then
            --     x:move(Direction.Right)
            -- end
            Framework:showFPS()
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
