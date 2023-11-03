require "Tools.ParseXML"
require "DataStage"
require "GameClear"
---@class GameStage
GameStage = {}
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
        obj:init(stageId)
        return obj
    end,
    __index = Object(),
    __tostring = function() return "GameStageClass" end
})

function GameStage:init(stageId)
    self._mStageId = stageId
    self._mViewClear = GameClear()
    self:loadStage(stageId)
    self._mSprites = Image(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.png")
    local configObj = ParseXML(CMAKE_CURRENT_SOURCE_DIR .. "res/sprites.xml")
    self._mSpriteConfig = configObj:getData()
    local mWallSprite = self._mSpriteConfig:getChildByAttri("name", "Wall_Brown.png") or error()
    -- 单位长度
    self._unit = mWallSprite:getAttributeValue("width")
    -- 墙
    self._mWallSprite = {
        w = tonumber(mWallSprite:getAttributeValue("width")),
        h = tonumber(mWallSprite:getAttributeValue("height")),
        x = tonumber(mWallSprite:getAttributeValue("x")),
        y = tonumber(mWallSprite:getAttributeValue("y")),
        sprites = self._mSprites,
        drawAt = function(this, x, y)
            this.sprites:draw(x, y, this.x, this.y, this.w, this.h)
        end
    }
    -- 地面
    local mGroundSprit = self._mSpriteConfig:getChildByAttri("name", "Ground_Dirt.png") or error()
    self._mGroundSprite = {
        w = tonumber(mGroundSprit:getAttributeValue("width")),
        h = tonumber(mGroundSprit:getAttributeValue("height")),
        x = tonumber(mGroundSprit:getAttributeValue("x")),
        y = tonumber(mGroundSprit:getAttributeValue("y")),
        sprites = self._mSprites,
        drawAt = function(this, x, y)
            this.sprites:draw(x, y, this.x, this.y, this.w, this.h)
        end
    }
    -- 目标点
    local mGoalSprit = self._mSpriteConfig:getChildByAttri("name", "EndPoint_Yellow.png") or error()
    self._mGoalSprit = {
        w = tonumber(mGoalSprit:getAttributeValue("width")),
        h = tonumber(mGoalSprit:getAttributeValue("height")),
        x = tonumber(mGoalSprit:getAttributeValue("x")),
        y = tonumber(mGoalSprit:getAttributeValue("y")),
        sprites = self._mSprites,
        drawAt = function(this, x, y)
            this.sprites:draw(x, y, this.x, this.y, this.w, this.h)
        end
    }
    -- 正常箱子
    local mNormalBoxSprit = self._mSpriteConfig:getChildByAttri("name", "CrateDark_Beige.png") or error()
    self._mNormalBoxSprit = {
        w = tonumber(mNormalBoxSprit:getAttributeValue("width")),
        h = tonumber(mNormalBoxSprit:getAttributeValue("height")),
        x = tonumber(mNormalBoxSprit:getAttributeValue("x")),
        y = tonumber(mNormalBoxSprit:getAttributeValue("y")),
        sprites = self._mSprites,
        drawAt = function(this, x, y)
            this.sprites:draw(x, y, this.x, this.y, this.w, this.h)
        end
    }
    -- 目标点上箱子
    local mYellowBoxSprit = self._mSpriteConfig:getChildByAttri("name", "CrateDark_Yellow.png") or error()
    self._mYellowBoxSprit = {
        w = tonumber(mYellowBoxSprit:getAttributeValue("width")),
        h = tonumber(mYellowBoxSprit:getAttributeValue("height")),
        x = tonumber(mYellowBoxSprit:getAttributeValue("x")),
        y = tonumber(mYellowBoxSprit:getAttributeValue("y")),
        sprites = self._mSprites,
        drawAt = function(this, x, y)
            this.sprites:draw(x, y, this.x, this.y, this.w, this.h)
        end
    }
    -- 工人
    local mWorkerSprit = self._mSpriteConfig:getChildByAttri("name", "Character5.png") or error()
    self._mWorkerSprit = {
        w = tonumber(mWorkerSprit:getAttributeValue("width")),
        h = tonumber(mWorkerSprit:getAttributeValue("height")),
        x = tonumber(mWorkerSprit:getAttributeValue("x")),
        y = tonumber(mWorkerSprit:getAttributeValue("y")),
        sprites = self._mSprites,
        drawAt = function(this, x, y)
            this.sprites:draw(x, y, this.x, this.y, this.w, this.h)
        end
    }
end

function GameStage:isClear()
    for _, box in ipairs(self._boxes) do
        if not self:isGoal(box.x, box.y) then
            return false
        end
    end
    return true
end

function GameStage:getBoxAt(x, y)
    for _, box in ipairs(self._boxes) do
        if box.x == x and box.y == y then
            return box
        end
    end
    error("not a box")
end

function GameStage:canMoveFromTo(x, y, dx, dy)
    local targetX = x + dx
    local targetY = y + dy
    local subTargetX = targetX + dx
    local subTargetY = targetY + dy
    if self:isWall(targetX, targetY) then
        return false
    end
    if self:isBox(targetX, targetY) then
        if self:isWall(subTargetX, subTargetY) or self:isBox(subTargetX, subTargetY) then
            return false
        end
    end
    return true
end

function GameStage:move(dx, dy)
    if self:canMoveFromTo(self._worker.x, self._worker.y, dx, dy) then
        local targetX = self._worker.x + dx
        local targetY = self._worker.y + dy
        if self:isBox(targetX, targetY) then
            local subTargetX = targetX + dx
            local subTargetY = targetY + dy
            local box = self:getBoxAt(targetX, targetY)
            box.x = subTargetX
            box.y = subTargetY
        end

        self._worker.x = targetX
        self._worker.y = targetY
    end
end

function GameStage:isBox(x, y)
    for _, box in ipairs(self._boxes) do
        if box.x == x and box.y == y then
            return true
        end
    end
    return false
end

function GameStage:isWall(x, y)
    if y > #self._background or y < 1 then
        return true
    end
    local row = self._background[y]
    if x > #row or x < 1 then
        return true
    end
    local obj = row[x]
    if obj == Enum.Obj.wall then
        return true
    end
    return false
end

function GameStage:update()
    if self:isClear() then
        self:draw()
        if Framework:isKeyTriggered(Enum.Keyboard.Space) then
            if self._mStageId < DataStage:getStageCount() then
                self._mStageId = self._mStageId + 1
                self:init(self._mStageId)
            end
        end
        return
    end
    if Framework:isKeyTriggered(Enum.Keyboard.A) then
        self:move(-1, 0)
    elseif Framework:isKeyTriggered(Enum.Keyboard.S) then
        self:move(0, 1)
    elseif Framework:isKeyTriggered(Enum.Keyboard.D) then
        self:move(1, 0)
    elseif Framework:isKeyTriggered(Enum.Keyboard.W) then
        self:move(0, -1)
    end
    self:draw()
end

function GameStage:draw()
    self:drawBackground()
    self:drawBoxes()
    self:drawWorker()
    if self:isClear() then
        self._mViewClear:update()
    end
end

function GameStage:drawWorker()
    local x = (self._worker.x - 0.5) * self._unit - 0.5 * self._mWorkerSprit.w
    local y = self._worker.y * self._unit - self._mWorkerSprit.h
    self._mWorkerSprit:drawAt(x, y)
end

---comment
---@param stageId integer
function GameStage:loadStage(stageId)
    local stage = DataStage:getStageById(stageId)
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
    self._worker = {}
    for k, v in pairs(stage.worker) do
        self._worker[k] = v
    end
end

function GameStage:drawBoxes()
    for _, box in ipairs(self._boxes) do
        if self:isGoal(box.x, box.y) then
            self._mYellowBoxSprit:drawAt((box.x - 1) * self._unit, (box.y - 1) * self._unit)
        else
            self._mNormalBoxSprit:drawAt((box.x - 1) * self._unit, (box.y - 1) * self._unit)
        end
    end
end

function GameStage:isGoal(x, y)
    for _, goal in ipairs(self._goals) do
        if goal.x == x and goal.y == y then
            return true
        end
    end
    return false
end

function GameStage:drawBackground()
    for y, row in ipairs(self._background) do
        for x, o in ipairs(row) do
            if o == Enum.Obj.ground then
                self._mGroundSprite:drawAt((x - 1) * self._unit, (y - 1) * self._unit)
            elseif o == Enum.Obj.wall then
                self._mWallSprite:drawAt((x - 1) * self._unit, (y - 1) * self._unit)
            end
        end
    end
    for _, goal in ipairs(self._goals) do
        self._mGoalSprit:drawAt((goal.x - 0.75) * self._unit, (goal.y - 0.75) * self._unit)
    end
end
