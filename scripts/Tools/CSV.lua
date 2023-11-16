require "Tools.Parser"
---@class CSV:Parser
---@field _mData CSVCell[][]
CSV = {
    __parent = Parser
}

setmetatable(CSV, {
    __call = function(self, pathOrStream)
        ---@class CSV
        local obj = {}
        setmetatable(obj, { __index = self })
        obj:_construct(pathOrStream)
        return obj
    end,
    __index = CSV.__parent
})
function CSV:_construct(pathOrStream)
    CSV.__parent._construct(self, pathOrStream) -- 调用父类的构建函数
    -- 如果加上这个, 对于空矩来说计算量太大了,所以先假设第一行都是对齐的
    self._mCsvRowNumber = 0
    self._mCsvColNumber = 0
    self._mData = {}
    if pathOrStream then
        self:_parse()
    end
end

function CSV:getData()
    return self._mData
end

function CSV:_parse()
    repeat
        local row = {}
        while self._mCurrentChar ~= "\n" do
            row[#row + 1] = self:_readString()
        end
        self._mData[#self._mData + 1] = row
        self:_getNextChar()
    until self:_isEnd()
end

function CSV:writeTo(path)
    local file = io.open(path, "w") or error("can't open " .. path)
    for _, row in ipairs(self._mData) do
        for i, cell in ipairs(row) do
            file:write(cell:getRawData())
            if i < #row then
                file:write(",")
            end
        end
        file:write("\n")
    end
    file:close()
end

---@param data string|nil
---@return CSVCell
function CSV:_spawnCell(data)
    ---@class CSVCell
    local cell = {
        getData = function(this)
            return this._mData
        end,
        setData = function(this, mData)
            if string.match(mData, '["|,]') then
                this._useQuotation = true
            end
            this._mData = mData
        end,
        isUseQuotation = function(this)
            return this._useQuotation
        end,
        getRawData = function(this)
            if this._useQuotation then
                return string.format('"%s"', string.gsub(this._mData, '"', '""'))
            else
                return this._mData
            end
        end,
        _useQuotation = false,
        _mData = ""
    }
    if data then
        cell:setData(data)
    end
    return cell
end

function CSV:setCell(row, col, data)
    self._mData[row] = self._mData[row] or {}
    local cell = self._mData[row][col]
    if cell then
        cell:setData(data)
    else
        self._mData[row][col] = self:_spawnCell(data)
    end
end

function CSV:_readString()
    local cell = self:_spawnCell()
    local s = {}
    -- 以 " 开头
    if self._mCurrentChar == '"' then
        self:_getNextChar() -- 跳过 "
        while true do
            if self._mCurrentChar == '"' then
                if self:_checkNextChar('"') then
                    self:_getNextChar() -- 跳过第一个 "
                else
                    break
                end
            end
            s[#s + 1] = self._mCurrentChar
            self:_getNextChar()
        end
        self:_getNextChar() -- 跳过 "
    else
        while self._mCurrentChar ~= "," and self._mCurrentChar ~= "\n" do
            s[#s + 1] = self._mCurrentChar
            self:_getNextChar()
        end
    end
    if self._mCurrentChar == "," then
        self:_getNextChar() -- 吞掉 ,
    end
    cell:setData(table.concat(s))
    return cell
end
