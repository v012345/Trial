local Stage = {}
function Stage:init()
    print("Stage init")
end

function Stage:destroy()
    print("Stage destroy")
end

function Stage:update()
    Impl:clear()
    Framework:string("请选关", 10, 10, 30, 30, 0xffffff)
    if Framework.isKeyTriggered(Keyboard.Space) then
        SceneManager:load("Game")
    end
end

return Stage
