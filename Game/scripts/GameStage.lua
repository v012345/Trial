require "Tools.ParseXML"
require "DataStage"
---@class GameStage
GameStage = {
    mSpriteConfig = (ParseXML(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.xml")):getData()
}
setmetatable(GameStage, {
    __call = function(self, stageId)
        ---@class GameStage
        ---@field mImage Image
        ---@field mStage StageData
        local obj = {}
        setmetatable(obj, {
            __index = self,
            __tostring = function() return "GameStageObject" end
        })
        obj:loadStage(stageId)
        obj.mGroundSprite = obj:getSprite("Ground_Dirt.png")
        obj.mWallSprite = obj:getSprite("Wall_Brown.png")
        obj.mGoalSprite = obj:getSprite("EndPoint_Yellow.png")
        obj.mBoxSprite = obj:getSprite("CrateDark_Beige.png")
        obj.mBoxOneGoalSprite = obj:getSprite("CrateDark_Yellow.png")
        obj.mSprite = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.png")
        obj.count = 0
        obj._moveLength = obj.mGroundSprite.w
        obj.mWorker = obj:loadWorker(obj.mSprite, obj.mGroundSprite.w, stageId)
        return obj
    end,
    __index = Object(),
    __tostring = function() return "GameStageClass" end
})

function GameStage:loadWorker(mSprite, length, stageId)
    local stage = DataStage.stage[stageId]
    local worker = {
        isMoving = false,
        x = stage.worker.x,
        y = stage.worker.y,
        counter = 1,
        down = {
            self:getSprite("Character4.png"),
            self:getSprite("Character5.png"),
            self:getSprite("Character6.png"),
        },
        up = {
            self:getSprite("Character7.png"),
            self:getSprite("Character8.png"),
            self:getSprite("Character9.png"),
        },
        left = {
            self:getSprite("Character1.png"),
            self:getSprite("Character10.png"),
        },
        right = {
            self:getSprite("Character2.png"),
            self:getSprite("Character3.png"),
        },
        toward = "down",
        mSprite = mSprite
    }

    function worker:moveUp() -- 7 8 9
        if not self.isMoving then
            self.isMoving = true
        else
            self.toward = "up"
        end
    end

    function worker:moveDown() -- 4 5 6
        if not self.isMoving then
            self.isMoving = true
        else
            self.toward = "down"
        end
    end

    function worker:moveLeft() -- 1 10
        if not self.isMoving then
            self.isMoving = true
        else
            self.toward = "left"
        end
    end

    function worker:moveRight() -- 2 3
        if not self.isMoving then
            self.isMoving = true
        else
            self.toward = "right"
        end
    end

    function worker:draw()
        self.counter = self.counter + 1
        if self.isMoving then
        else
            local congig = self[self.toward][1]
            self.mSprite:draw((self.x - 1 + 0.5) * length - congig.w, (self.y - 1 + 1) * length - congig.h,
                congig.x,
                congig.y,
                congig.w,
                congig.h)
        end
    end

    return worker
end

function GameStage:isClear()
    return false
end

function GameStage:update()
    if Framework:isKeyTriggered(Enum.Keyboard.Space) then
        self.count = self.count + 1
    elseif Framework:isKeyTriggered(Enum.Keyboard.A) then
        self.mWorker:moveLeft()
    elseif Framework:isKeyTriggered(Enum.Keyboard.S) then
        self.mWorker:moveDown()
    elseif Framework:isKeyTriggered(Enum.Keyboard.D) then
        self.mWorker:moveRight()
    elseif Framework:isKeyTriggered(Enum.Keyboard.W) then
        self.mWorker:moveUp()
    end
    self:draw()
end

function GameStage:draw()
    -- self:drawSprite(self.count)
    self:drawBackground()
    self:drawBoxes()
    self.mWorker:draw()
end

function GameStage:drawSprite(count)
    local id = count % #self.mSpriteConfig.children + 1
    local s = self.mSpriteConfig.children[id]
    local x = s.attributes.x
    local y = s.attributes.y
    local w = s.attributes.width
    local h = s.attributes.height
    local c = s.attributes.name
    self.mSprite:draw(0, 0, x, y, w, h)
    Framework:drawDebugString(20, 20, c)
end

---comment
---@param stageId integer
function GameStage:loadStage(stageId)
    local stage = DataStage.stage[stageId]
    self._background = stage.background
    self._goals = {}
    for _, goal in ipairs(stage.goals) do
        local _goal = {}
        for k, v in pairs(goal) do
            _goal[k] = v
        end
        self._goals[#self._goals + 1] = _goal
    end
    self._boxes = {}
    for _, box in ipairs(stage.boxes) do
        local _box = {}
        for k, v in pairs(box) do
            _box[k] = v
        end
        self._boxes[#self._boxes + 1] = _box
    end
end

function GameStage:getSprite(which)
    for _, sprite in ipairs(self.mSpriteConfig.children) do
        if sprite.attributes.name == which then
            return {
                x = sprite.attributes.x,
                y = sprite.attributes.y,
                w = sprite.attributes.width,
                h = sprite.attributes.height
            }
        end
    end
    error(which)
end

function GameStage:drawBoxes()
    for _, box in ipairs(self._boxes) do
        self.mSprite:draw((box.x - 1) * self.mBoxSprite.w, (box.y - 1) * self.mBoxSprite.h,
            self.mBoxSprite.x,
            self.mBoxSprite.y,
            self.mBoxSprite.w,
            self.mBoxSprite.h)
    end
end

function GameStage:drawBackground()
    for y, row in ipairs(self._background) do
        for x, o in ipairs(row) do
            if o == Enum.Obj.ground then
                self.mSprite:draw((x - 1) * self.mGroundSprite.w, (y - 1) * self.mGroundSprite.h,
                    self.mGroundSprite.x,
                    self.mGroundSprite.y,
                    self.mGroundSprite.w,
                    self.mGroundSprite.h)
            elseif o == Enum.Obj.wall then
                self.mSprite:draw((x - 1) * self.mWallSprite.w, (y - 1) * self.mWallSprite.h,
                    self.mWallSprite.x,
                    self.mWallSprite.y,
                    self.mWallSprite.w,
                    self.mWallSprite.h)
            end
        end
    end
    for _, goal in ipairs(self._goals) do
        self.mSprite:draw((goal.x - 1 + 0.25) * self.mWallSprite.w, (goal.y - 1 + 0.25) * self.mWallSprite.h,
            self.mGoalSprite.x,
            self.mGoalSprite.y,
            self.mGoalSprite.w,
            self.mGoalSprite.h)
    end
end

function GameStage:isA(what)
    if GameStage == what then
        return true
    else
        local super = getmetatable(self)
        if super then
            return super:isA(what)
        else
            return false
        end
    end
end
