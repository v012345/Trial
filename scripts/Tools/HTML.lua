require "Tools.Parser"
---@class HTML:Parser 不完善的 HTML 解析器
HTML = {
    __parent = Parser
}

setmetatable(HTML, {
    __call = function(self, pathOrStream)
        ---@class HTML
        local obj = {}
        setmetatable(obj, { __index = self })
        obj:_construct(pathOrStream)
        return obj
    end,
    __index = HTML.__parent
})

function HTML:_construct(pathOrStream)
    HTML.__parent._construct(self, pathOrStream) -- 调用父类的构建函数
    self._mData = self:newNode()
    if pathOrStream then
        self:_parse()
    end
end

---@return HTMLNode
function HTML:getData()
    return self._mData
end

---@param path string
function HTML:writeTo(path)
    local file = io.open(path, "w") or error("can't open " .. path)
    ---@param node HTMLNode
    local function dump(node)
        file:write("<", node:getTagName())
        for _, key in ipairs(node:getSortedAttriKey()) do
            file:write(string.format(' %s="%s"', key, node:getAttributeValue(key)))
        end
        local children = node:getChildren()
        if #children > 0 then
            file:write(">\n")
            for _, child in ipairs(node:getChildren()) do
                dump(child)
            end
            file:write("</", node:getTagName(), ">\n")
        else
            file:write(" />\n")
        end
    end
    dump(self._mData)
    file:close()
end

function HTML:_parse()
    self:_skipSpace()
    self._mData = self:_parserNode()
end

---comment
---@param tagName string|nil
---@return HTMLNode
function HTML:newNode(tagName)
    ---@class HTMLNode
    ---@field _tag_name string
    ---@field _attributes table<string,string>
    ---@field _children HTMLNode[]
    ---@field _content string
    ---@field _attris_key_sorted string[]
    local node = {
        _tag_name = tagName or "",
        _attributes = {},
        _attris_key_sorted = {},
        _children = {},
        _content = "",
        ---@param this HTMLNode
        ---@return HTMLNode[]
        getChildren = function(this)
            return this._children
        end,
        ---@param this HTMLNode
        ---@return HTMLNode
        getChild = function(this, idx)
            return this._children[idx]
        end,
        ---@param this HTMLNode
        addChild = function(this, HTMLNode)
            this._children[#this._children + 1] = HTMLNode
        end,
        ---@param this HTMLNode
        ---@param key string
        ---@return string|nil
        getAttributeValue = function(this, key)
            return this._attributes[key]
        end,
        ---@param this HTMLNode
        ---@param key string
        ---@param value any
        setAttributeValue = function(this, key, value)
            if not this._attributes[key] then
                this._attris_key_sorted[#this._attris_key_sorted + 1] = key
            end
            this._attributes[key] = tostring(value)
        end,
        ---@param this HTMLNode
        ---@return table<string,string>
        getAttributes = function(this)
            return this._attributes
        end,
        ---@param this HTMLNode
        ---@return string[]
        getSortedAttriKey = function(this)
            return this._attris_key_sorted
        end,
        ---@param this HTMLNode
        ---@return string
        getTagName = function(this)
            return this._tag_name
        end,
        ---@param this HTMLNode
        ---@param key string
        ---@param value string
        ---@return HTMLNode|nil
        getChildByAttri = function(this, key, value)
            for _, child in ipairs(this._children) do
                if child._attributes[key] == value then
                    return child
                end
            end
            return nil
        end
    }
    return node
end

function HTML:_parserNode()
    self:_getNextChar() -- 跳过 <
    local node = self:newNode()
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
            if node._tag_name == "img" then
                return node
            end
        else
            local key, value = self:_readAttri()
            if key then
                node._attributes[key] = value
                node._attris_key_sorted[#node._attris_key_sorted + 1] = key
            else
                error("miss key")
            end
        end
    end
end

function HTML:_readAttri()
    local key = self:_readName()
    self:_skipSpace()
    if self._mCurrentChar ~= "=" then
        error("miss = @ " .. self._mCharPointer)
    end
    self:_getNextChar() -- 跳过 =
    local value = self:_readString()
    return key, value
end

function HTML:_readString()
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

function HTML:_readName()
    self:_skipSpace()
    local s = {}
    while string.match(self._mCurrentChar, "[%w_]") do
        s[#s + 1] = self._mCurrentChar
        self:_getNextChar()
    end
    if #s == 0 then
        print(self._mCurrentChar)
        error(string.format("no name @ row %s col %s \n %s", self._mRowNum, self._mColNum, debug.traceback()))
    end
    return table.concat(s)
end
