local Menu = {}
function Menu:init()
    print("Menu init")
end

function Menu:destroy()
    print("Menu destroy")
end

function Menu:update()
    -- Impl:clear()
    Framework:string("我要干什么呢", 50, 50, 30, 30, 0xff0000)
    if Framework.isKeyTriggered(Keyboard.Q) then
        ViewManager:pop()
    elseif Framework.isKeyTriggered(Keyboard.S) then
        ViewManager:pop()
        SceneManager:load("Stage")
    end
end

return Menu
