local Loading = {}
function Loading:init()
    print("Loading init")
    self.isLoading = true
    self.counter = 0
end

function Loading:checkLoading()
    return self.isLoading
end

function Loading:destroy()
    print("Loading destroy")
end

function Loading:update()
    Impl:clear()
    Framework:string(string.format("加载中%s%%", self.counter), 10, 10, 20, 20, 0xffffff)
    self.counter = self.counter + 10
    if self.counter > 100 then
        self.isLoading = false
    end
end

return Loading
