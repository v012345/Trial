require "Tools.Object"
---@class Parser:Object
Parser = {
    __parent = Object
}

setmetatable(Parser, {
    __call = function(self, pathOrStream)
        ---@class Parser
        local obj = {}
        setmetatable(obj, { __index = self })
        self:_construct(pathOrStream)
        return obj
    end,
    __index = Parser.__parent -- 继承 Object 中的方法
})

function Parser:_construct(pathOrStream)
    Parser.__parent._construct(self)
    if pathOrStream then
        local file = io.open(pathOrStream, "r")
        if file then
            local stream = file:read("a")
            file:close()
            if #stream > 3 then
                local bom = string.format("%x%x%x", string.byte(stream, 1, 3))
                if string.lower(bom) == "efbbbf" then
                    stream = string.sub(stream, 4, #stream)
                end
            end
            self._mStream = stream
        else
            self._mStream = pathOrStream
        end

        self._mCharPointer = 0
        self._mRowNum = 1
        self._mColNum = 0
        self:_getFirstChar()
    end
end

function Parser:writeTo(path)
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

function Parser:_isEnd()
    return self._mCharPointer > #self._mStream
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
