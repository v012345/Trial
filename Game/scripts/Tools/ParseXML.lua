---@class ParseXML
ParseXML = {}

setmetatable(ParseXML, {
    __call = function(self, path)
        ---@class ParseXML
        local obj = {}
        setmetatable(obj, { __index = self })
        obj:init(path)
        obj:_parse()
        return obj
    end,
    __index = Object()
})

function ParseXML:init(path)
    local file = io.open(path, "r") or error("can't open " .. path)
    self._mXmlString = file:read("a")
    file:close()
    self._mCharPointer = 0
end

---@return XMLNode
function ParseXML:getData()
    return self.mData
end

function ParseXML:_isSpace()
    local space = {
        [" "] = " ",
        ["\t"] = "\t",
        ["\n"] = "\n",
        ["\f"] = "\f",
        ["\v"] = "\v",
    }
    return space[self._mCurrentChar]
end

function ParseXML:_checkNextChar(c)
    return string.sub(self._mXmlString, self._mCharPointer + 1, self._mCharPointer + 1) == c
end

function ParseXML:_parse()
    self:_getFirstChar()
    self:_skipSpace()
    self.mData = self:_parserNode()
end

function ParseXML:_getFirstChar()
    self._mCharPointer = self._mCharPointer + 1
    self._mCurrentChar = string.sub(self._mXmlString, self._mCharPointer, self._mCharPointer)
    return self._mCurrentChar
end

function ParseXML:_parserNode()
    self:_getNextChar() -- 跳过 <
    ---@class XMLNode
    ---@field _tag_name string
    ---@field _attributes table<string,string>
    ---@field _children XMLNode[]
    ---@field _content string
    local node = {
        _tag_name = "",
        _attributes = {},
        _children = {},
        _content = "",
        ---@param this XMLNode
        ---@return XMLNode[]
        getChildren = function(this)
            return this._children
        end,
        ---@param this XMLNode
        ---@param key string
        ---@return string|nil
        getAttributeValue = function(this, key)
            return this._attributes[key]
        end,
        ---@param this XMLNode
        ---@return table<string,string>
        get_attributes = function(this)
            return this._attributes
        end,
        ---@param this XMLNode
        ---@param key string
        ---@param value string
        ---@return XMLNode|nil
        getChildByAttri = function(this, key, value)
            for _, child in ipairs(this._children) do
                if child._attributes[key] == value then
                    return child
                end
            end
            return nil
        end
    }
    node._tag_name = self:_readName()

    while true do
        if self._mCurrentChar == "<" then
            if self:_checkNextChar("/") then
                self:_getNextChar() -- 跳过 <
                self:_getNextChar() -- 跳过 /
                if node._tag_name ~= self:_readName() then
                    error("don't close tag")
                end
                self:_skipSpace()
                self:_getNextChar() -- 跳过 >
                return node
            else
                node._children[#node._children + 1] = self:_parserNode()
            end
        elseif self._mCurrentChar == "/" then
            self:_getNextChar()
            self:_skipSpace()
            if self._mCurrentChar == ">" then
                self:_getNextChar() -- 跳过 >
                return node
            else
                error(self._mCurrentChar)
            end
        elseif self:_isSpace() then
            self:_skipSpace()
        elseif self._mCurrentChar == ">" then
            self:_getNextChar() -- 跳过 >
        else
            local key, value = self:_readAttri()
            if key then
                node._attributes[key] = value
            else
                error("miss key")
            end
        end
    end
end

function ParseXML:_readAttri()
    local key = self:_readName()
    self:_skipSpace()
    if self._mCurrentChar ~= "=" then
        error("miss = @ " .. self._mCharPointer)
    end
    self:_getNextChar() -- 跳过 =
    local value = self:_readString()
    return key, value
end

function ParseXML:_readString()
    self:_skipSpace()
    if self._mCurrentChar == '"' then
        self:_getNextChar() -- 跳过 "
        local s = {}
        while true do
            if self._mCurrentChar == '"' then
                break
            end
            s[#s + 1] = self._mCurrentChar
            self:_getNextChar()
        end
        self:_getNextChar() -- 跳过 "
        return table.concat(s)
    else
        error("miss \"")
    end
end

function ParseXML:_getNextChar()
    self._mCharPointer = self._mCharPointer + 1
    self._mCurrentChar = string.sub(self._mXmlString, self._mCharPointer, self._mCharPointer)
    return self._mCurrentChar
end

function ParseXML:_skipSpace()
    while self:_isSpace() do
        self._mCurrentChar = self:_getNextChar()
    end
    return self._mCurrentChar
end

function ParseXML:_readName()
    self:_skipSpace()
    local s = {}
    while string.match(self._mCurrentChar, "%w") do
        s[#s + 1] = self._mCurrentChar
        self:_getNextChar()
    end
    if #s == 0 then
        error("no name" .. debug.traceback())
    end
    return table.concat(s)
end
