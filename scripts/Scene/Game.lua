local Game = {}
function Game:init()
    print("Game init")
    self.map = ReadPngFile(CMAKE_SOURCE_DIR .. "res/shengyue.png")
end

function Game:destroy()
    print("Game destroy")
end

function Game:update()
    Impl:clear()
    Framework:string("正在游戏中", 10, 10, 30, 30, 0xffffff)
    Framework:draw(self.map, 50, 50, true)
end

return Game
