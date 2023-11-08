require "Tools.Parser"
---@class ParseCSV:Parser
ParseCSV = {}

setmetatable(ParseCSV, {
    __call = function(self, path)
        ---@class ParseCSV
        local obj = {}
        setmetatable(obj, { __index = self })
        obj:init(path)
        obj:_parse()
        return obj
    end,
    __index = Parser()
})
function ParseCSV:getData()
    return self._mData
end

function ParseCSV:_parse()
    self._mData = {}
    repeat
        local row = {}
        while self._mCurrentChar ~= "\n" do
            row[#row + 1] = self:_readString()
        end
        self._mData[#self._mData + 1] = row
        self:_getNextChar()
    until self:_isEnd()
    print(#self._mData)
end

function ParseCSV:writeTo(path)
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

function ParseCSV:_readString()
    ---@class CSVCell
    local cell = {
        getData = function(this)
            return this._mData
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
        end
    }
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
        cell._useQuotation = true
    else
        while self._mCurrentChar ~= "," and self._mCurrentChar ~= "\n" do
            s[#s + 1] = self._mCurrentChar
            self:_getNextChar()
        end
        cell._useQuotation = false
    end
    if self._mCurrentChar == "," then
        self:_getNextChar() -- 吞掉 ,
    end
    cell._mData = table.concat(s)
    return cell
end
