local Menu = {}
function Menu:init()
    print("Menu init")
end

function Menu:destroy()
    print("Menu destroy")
end

function Menu:update()
    Impl:clear()
    Framework:string("正在游戏中", 10, 10, 30, 30, 0xffffff)
end

return Menu
