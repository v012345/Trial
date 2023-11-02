require "ScenceTitle"
---@class Director
Director = {
    MODE_NONE = 0,
    Mode = {
        MODE_1P = 1,
        MODE_2P = 2,
        MODE_NONE = 0,
    },
    mCurrentScence = ScenceTitle()
}

function Director:update()
    self.mCurrentScence:update(self);
end

function Director:changeScence(which)
    self.mCurrentScence = which
end

function Director:getMode()
    return self.mMode
end

function Director:setMode(mode)
    self.mMode = mode
end

function Director:mode()
    return self:getMode()
end

setmetatable(Director, {
    __tostring = function() return "Director" end
})
