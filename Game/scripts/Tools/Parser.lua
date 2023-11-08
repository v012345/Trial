require "Object"
---@class Parser
Parser = {}

setmetatable(Parser, {
    __call = function(self)
        ---@class Parser
        local obj = {}
        setmetatable(obj, { __index = self })
        return obj
    end,
    __index = Object()
})

function Parser:init(path)
    local file = io.open(path, "r") or error("can't open " .. path)
    self._mStream = file:read("a")
    file:close()
    self._mCharPointer = 0
    self._mRowNum = 1
    self._mColNum = 0
end

function Parser:writeTo()
    error("must implemention")
end

function Parser:_isSpace()
    local space = {
        [" "] = " ",
        ["\t"] = "\t",
        ["\n"] = "\n",
        ["\f"] = "\f",
        ["\v"] = "\v",
    }
    return space[self._mCurrentChar]
end

function Parser:_checkNextChar(c)
    return string.sub(self._mStream, self._mCharPointer + 1, self._mCharPointer + 1) == c
end

function Parser:_getFirstChar()
    return self:_getNextChar()
end

function Parser:_getNextChar()
    self._mCharPointer = self._mCharPointer + 1
    self._mCurrentChar = string.sub(self._mStream, self._mCharPointer, self._mCharPointer)
    self._mColNum = self._mColNum + 1
    if self._mCurrentChar == "\n" then
        self._mRowNum = self._mRowNum + 1
        self._mColNum = 0
    end
    return self._mCurrentChar
end

function Parser:_skipSpace()
    while self:_isSpace() do
        self._mCurrentChar = self:_getNextChar()
    end
    return self._mCurrentChar
end
