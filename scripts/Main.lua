xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "init"
    local Link = require "Config.Link"
    ViewManager:init()
    SceneManager:load("Title")
    local x = SpriteManager:create(Link)
    function MainLoop()
        xpcall(function()
            Framework:fixFPS()
            -- SceneManager:update()
            -- ViewManager:update()
            Impl:clear()
            -- sprite.count = (sprite.count) % 10 + 1
            -- Framework:draw(sprite.x[sprite.count], 0, 0, false)
            x:update()
            if Framework.isKeyOn(Keyboard.A) then
                x:move(Direction.Left)
            end
            if Framework.isKeyOn(Keyboard.S) then
                x:move(Direction.Down)
            end
            if Framework.isKeyOn(Keyboard.D) then
                x:move(Direction.Right)
            end
            if Framework.isKeyOn(Keyboard.W) then
                x:move(Direction.Up)
            end
            Framework:showFPS()
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
