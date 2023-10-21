local Game = {}
function Game:init()
    print("Game init")
    self.map = ReadPngFile(CMAKE_SOURCE_DIR .. "res/shengyue.png")
    self.showMenu = false
    self.Menu = require "Views.Menu"
end

function Game:destroy()
    print("Game destroy")
    package.loaded["Views.Menu"] = nil
end

function Game:update()
    Impl:clear()
    Framework:string("正在游戏中", 10, 10, 30, 30, 0xffffff)
    Framework:draw(self.map, 50, 50, true)
    if Framework.isKeyTriggered(Keyboard.E) then
        ViewManager:show("Menu")
    end
end

return Game
