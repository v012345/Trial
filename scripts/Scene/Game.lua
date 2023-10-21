local Game = {}
function Game:init()
    print("Game init")
end

function Game:destroy()
    print("Game destroy")
end

function Game:update()
    Impl:clear()
    Framework:string("正在游戏中", 10, 10, 30, 30, 0xffffff)
    
end

return Game
