local Title = {}
function Title:init()
    print("Title init")
end

function Title:destroy()
    print("Title destroy")
end

function Title:update()
    Impl:clear()
    Framework:string("欢迎游玩推箱子", 10, 10, 30, 30, 0xffffff)
    if Framework.isKeyTriggered(Keyboard.Space) then
        SceneManager:load("Stage")
    end
end

return Title
