ViewManager = {}
function ViewManager:init()
    self.mViews = {}
end

function ViewManager:show(path)
    local view = require("Views." .. path)
    for _, v in ipairs(self.mViews) do
        if v == view then
            return
        end
    end
    self.mCurrentView = view
    self.mViews[#self.mViews + 1] = view
    self.mCurrentView:init()
    return view
end

function ViewManager:pop()
    return table.remove(self.mViews)
end

function ViewManager:update()
    local view = self.mViews[#self.mViews]
    if view then
        view:update()
    end
end
