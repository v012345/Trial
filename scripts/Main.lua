xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    require "init"
    Framework:init()
    local BlueManSprite = Sprite:new(require("Config.BlueMan"))
    local BlueManEntity = Entity:new({
        Sprite = BlueManSprite
    })
    ---@diagnostic disable-next-line
    function BlueManEntity:dealInput()
        local direction = Direction.Null
        if Framework.isKeyOn(Keyboard.W) then
            direction = direction | Input[Keyboard.W]
        end
        if Framework.isKeyOn(Keyboard.S) then
            direction = direction | Input[Keyboard.S]
        end
        if Framework.isKeyOn(Keyboard.A) then
            direction = direction | Input[Keyboard.A]
        end
        if Framework.isKeyOn(Keyboard.D) then
            direction = direction | Input[Keyboard.D]
        end
        self.Sprite:setDiretion(InputToDirection[direction])
        if direction == Direction.Null then
            self.Sprite:setAction("idle")
        else
            if Framework.isKeyOn(Keyboard.Shift) then
                self.Sprite:setAction("run")
            else
                self.Sprite:setAction("walk")
            end
        end
    end

    function MainLoop()
        xpcall(function()
            Framework:fixFPS()
            Impl:clear()
            BlueManEntity:update()
            Framework:showFPS()
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
