require "Tools.Parser"
---@class JSON:Parser
JSON = {
    escape = {
        ["\\"] = "\\",
        ["\""] = "\"",
        ["/"] = "/",
        ["r"] = "\r",
        ["f"] = "\f",
        ["n"] = "\n",
        ["t"] = "\t",
        ["b"] = "\b",
    },
    escape_r = {
        ["\\"] = "\\\\",
        ["\""] = "\\\"",
        ["/"] = "\\/",
        ["\r"] = "\\r",
        ["\f"] = "\\f",
        ["\n"] = "\\n",
        ["\t"] = "\\t",
        ["\b"] = "\\b",
    },
    __parent = Parser
}

setmetatable(JSON, {
    __call = function(self, pathOrStream)
        ---@class JSON
        local obj = {}
        setmetatable(obj, { __index = self })
        obj:_construct(pathOrStream)
        return obj
    end,
    __index = JSON.__parent
})

function JSON:_construct(pathOrStream)
    JSON.__parent._construct(self, pathOrStream) -- 调用父类的构建函数
    self._mData = {}
    if pathOrStream then
        self:_parse()
    end
end

function JSON:getData()
    return self._mData
end

function JSON:_parse()
    self:_skipSpace() -- 跳过文件开头空白
    if self._mCurrentChar == "{" then
        self._mData = self:_readObject()
    elseif self._mCurrentChar == "[" then
        self._mData = self:_readArray()
    elseif self._mCurrentChar == '"' then
        self._mData = self:_readString()
    else
        self._mData = self:_readBaseType()
    end
end

function JSON:_readArray()
    self:_getNextChar() -- 跳过 [
    self:_skipSpace()   -- 跳过 [ 后的空白
    local mt = {
        is_array = true
    }
    local result = {}
    setmetatable(result, mt)
    while true do
        if self._mCurrentChar == '"' then
            result[#result + 1] = self:_readString()
        elseif self:_isSpace() then
            self:_getNextChar()
        elseif self._mCurrentChar == "{" then
            result[#result + 1] = self:_readObject()
        elseif self._mCurrentChar == "," then
            self:_getNextChar() -- 跳过 ,
            self:_skipSpace()
            if self._mCurrentChar == "]" then
                error("the last element has a , follow")
            end
        elseif self._mCurrentChar == "]" then
            break
        elseif self._mCurrentChar == "[" then
            result[#result + 1] = self:_readArray()
        else
            result[#result + 1] = self:_readBaseType()
        end
    end
    self:_getNextChar() -- 跳过 ]
    self:_skipSpace()   -- 跳过文件结束空白
    if self._mCurrentChar == "," then
        self:_getNextChar()
    end
    return result
end

function JSON:_readString()
    local s = {}
    self:_getNextChar() --跳过开头 "
    while self._mCurrentChar ~= '"' do
        if self._mCurrentChar == "\\" then
            self:_getNextChar()
            local escape_char = JSON.escape[self._mCurrentChar]
            if escape_char then
                s[#s + 1] = escape_char
            else
                error(tostring(self._mCurrentChar) .. " can't escape")
            end
        else
            s[#s + 1] = self._mCurrentChar
        end
        self:_getNextChar()
    end
    self:_getNextChar() --跳过结尾 "
    return table.concat(s)
end

function JSON:_readK()
    return self:_readString()
end

function JSON:_readV()
    local value = nil
    if self._mCurrentChar == '"' then
        value = self:_readString()
    elseif self._mCurrentChar == "[" then
        value = self:_readArray()
    elseif self._mCurrentChar == "{" then
        value = self:_readObject()
    else
        value = self:_readBaseType()
    end
    return value
end

function JSON:_readKV()
    local key = self:_readK()
    self:_skipSpace()
    if self._mCurrentChar ~= ":" then
        error("miss \":\"")
        return
    end
    self:_getNextChar() -- 跳过 :
    self:_skipSpace()
    local value = self:_readV()
    self:_skipSpace()
    return key, value
end

function JSON:_readObject()
    self:_getNextChar() -- 跳过 {
    self:_skipSpace()   -- 跳过 { 后的空白
    local len = 0
    local result = {}
    local key_table = {} -- 防止 key 重复
    local key, value = nil, nil
    while true do
        if self._mCurrentChar == '"' then
            key, value = self:_readKV()
            if key then
                if key_table[key] then
                    error("duplicate key")
                else
                    key_table[key] = true
                    result[key] = value
                    self:_skipSpace()
                    len = len + 1
                end
            else
                error("can't get a key")
            end
        elseif self._mCurrentChar == "," then
            self:_getNextChar() -- 跳过 ,
            self:_skipSpace()
        elseif self._mCurrentChar == "}" then
            self:_getNextChar() -- 跳过 }
            self:_skipSpace()   -- 跳过文件结束空白
            if self._mCurrentChar == "," then
                self:_getNextChar()
            end
            break
        else
            error("wrong json object")
        end
    end

    local mt = {
        is_array = false,
        len = len
    }
    setmetatable(result, mt)
    return result
end

function Parser:_readBaseType()
    local s = {}
    ---@type any
    local r = nil
    while string.find(self._mCurrentChar, "[0-9aeflnrstu%.%-]") do
        s[#s + 1] = self._mCurrentChar
        self:_getNextChar()
    end
    local token = table.concat(s)
    if token == "true" then
        r = true
    elseif token == "null" then
        r = nil
    elseif token == "false" then
        r = false
    else
        local n = tonumber(token) -- todo 这里有问题, 比 json 规定的数字范围大
        if n then
            r = n
        else
            error("not a base type")
        end
    end
    self:_skipSpace()
    return r
end

function JSON:toString()
    local r = {}
    local function toJson(t)
        local typ = type(t)
        if typ == "nil" then
            r[#r + 1] = "null"
        elseif typ == "number" then
            r[#r + 1] = tostring(t)
        elseif typ == "boolean" then
            r[#r + 1] = tostring(t)
        elseif typ == "string" then
            r[#r + 1] = string.format("%q", t)
        elseif typ == "table" then
            r[#r + 1] = "{"
            for key, value in pairs(t) do
                if type(key) == "number" then
                    r[#r + 1] = "\""
                    r[#r + 1] = key
                    r[#r + 1] = "\""
                else
                    r[#r + 1] = string.format("%q", key)
                end
                r[#r + 1] = ":"
                toJson(value)
                r[#r + 1] = ","
            end
            if next(t) then
                table.remove(r)
            end
            r[#r + 1] = "}"
        else
            error("can't convert to json")
        end
    end
    toJson(self._mData)
    return table.concat(r)
end

function JSON:writeTo(path)
    local r = {}
    local function toJson(t)
        local typ = type(t)
        if typ == "nil" then
            r[#r + 1] = "null"
        elseif typ == "number" then
            r[#r + 1] = tostring(t)
        elseif typ == "boolean" then
            r[#r + 1] = tostring(t)
        elseif typ == "string" then
            r[#r + 1] = string.format("%q", t)
        elseif typ == "table" then
            r[#r + 1] = "{"
            for key, value in pairs(t) do
                if type(key) == "number" then
                    r[#r + 1] = "\""
                    r[#r + 1] = key
                    r[#r + 1] = "\""
                else
                    r[#r + 1] = string.format("%q", key)
                end
                r[#r + 1] = ":"
                toJson(value)
                r[#r + 1] = ","
            end
            if next(t) then
                table.remove(r)
            end
            r[#r + 1] = "}"
        else
            error("can't convert to json")
        end
    end
    toJson(self._mData)
    local file = io.open(path, "w") or error("can't open " .. path)
    file:write(table.concat(r))
    file:close()
end
