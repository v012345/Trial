SceneManager = {
    mScenes = {},
    mLoading = require "Scene.Loading",
}
function SceneManager:init()
    self.mCurrentScence = self.mLoading
end

function SceneManager:load(path)
    if path == "Loading" then
        error("Can't load loading scene")
    end
    self.mLoading:init()
    for _, v in ipairs(self.mScenes) do
        local scene = package.loaded[v]
        scene:destroy()
        package.loaded[v] = nil
    end
    self.mScenes = {}
    local loadScene = "Scene." .. path
    self.mScenes[#self.mScenes + 1] = loadScene
    self.mCurrentScence = require(loadScene)
    self.mCurrentScence:init()
end

function SceneManager:update()
    if self.mLoading:checkLoading() then
        self.mLoading:update()
    else
        self.mCurrentScence:update()
    end
end
