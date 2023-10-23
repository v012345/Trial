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
            Impl:clear()
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
