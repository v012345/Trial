---@class Player
Player = {
    screenX = 0,
    screenY = 0,
    x = 0,
    y = 0,
    dx = 0,
    dy = 0,
    counter = 0
}
function Player:move(dx, dy)
    if self.counter <= 0 then
        self.dx = dx
        self.dy = dy
        self.counter = Game.SpriteSize
    end
end

function Player:update(dt)
    if self.counter >= 1 then
        self.screenX = self.screenX + self.dx
        self.screenY = self.screenY + self.dy
        self.counter = self.counter - 1
    else
        self.counter = 0
        self.dx = 0
        self.dy = 0
    end
    Game:drawImage(Game.renderEntity[ENUM.player], self.screenX, self.screenY, false)
end
