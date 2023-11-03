local Parser = {
    space = {
        [" "] = " ",
        ["\t"] = "\t",
        ["\n"] = "\n",
        ["\f"] = "\f",
        ["\v"] = "\v",
    }
}

local function parse(xml_string)
    Parser.char_pointer = 0
    Parser.xml_string = xml_string
    Parser:get_next_char()
    Parser:skip_space()
    return Parser:parser_a_node()
end
ParseXML = {}
setmetatable(ParseXML, {
    __call = function(self, path)
        local file = io.open(path, "r") or error("can't open " .. path)
        local xml_string = file:read("a")
        file:close()
        ---@class ParserXML
        local obj = {}
        obj.mData = parse(xml_string)
        function obj:isA(what)
            if ParseXML == what then
                return true
            else
                local super = getmetatable(self)
                if super then
                    return super.__index:isA(what)
                else
                    return false
                end
            end
        end

        function obj:getData()
            return self.mData
        end

        setmetatable(obj, { __index = Object() })
        return obj
    end
})

function Parser:check_next_char(c)
    return string.sub(self.xml_string, self.char_pointer + 1, self.char_pointer + 1) == c
end

function Parser:parser_a_node()
    self:get_next_char() -- 跳过 <
    local node = {
        tag_name = nil,
        attributes = {},
        children = {},
        content = ""
    }
    node.tag_name = self:parser_a_name()

    while true do
        if self.current_char == "<" then
            if self:check_next_char("/") then
                self:get_next_char() -- 跳过 <
                self:get_next_char() -- 跳过 /
                if node.tag_name ~= self:parser_a_name() then
                    error("don't close tag")
                end
                self:skip_space()
                self:get_next_char() -- 跳过 >
                return node
            else
                node.children[#node.children + 1] = self:parser_a_node()
            end
        elseif self.current_char == "/" then
            self:get_next_char()
            self:skip_space()
            if self.current_char == ">" then
                self:get_next_char() -- 跳过 >
                return node
            else
                error(self.current_char)
            end
        elseif self.space[self.current_char] then
            self:skip_space()
        elseif self.current_char == ">" then
            self:get_next_char() -- 跳过 >
        else
            local key, value = self:parser_a_key_value_pair()
            if key then
                node.attributes[key] = value
            else
                error("miss key")
            end
        end
    end
end

function Parser:parser_a_key_value_pair()
    local key = self:parser_a_name()
    self:skip_space()
    if self.current_char ~= "=" then
        error("miss = @ " .. self.char_pointer)
    end
    self:get_next_char() -- 跳过 =
    local value = self:parser_a_string()
    return key, value
end

function Parser:parser_a_string()
    self:skip_space()
    if self.current_char == '"' then
        self:get_next_char() -- 跳过 "
        local s = {}
        while true do
            if self.current_char == '"' then
                break
            end
            s[#s + 1] = self.current_char
            self:get_next_char()
        end
        self:get_next_char() -- 跳过 "
        return table.concat(s)
    else
        error("miss \"")
    end
end

function Parser:get_next_char()
    self.char_pointer = self.char_pointer + 1
    self.current_char = string.sub(self.xml_string, self.char_pointer, self.char_pointer)
    return self.current_char
end

function Parser:skip_space()
    while self.space[self.current_char] do
        self.current_char = self:get_next_char()
    end
    return self.current_char
end

function Parser:parser_a_name()
    self:skip_space()
    local s = {}
    while string.match(self.current_char, "%w") do
        s[#s + 1] = self.current_char
        self:get_next_char()
    end
    if #s == 0 then
        error("no name" .. debug.traceback())
    end
    return table.concat(s)
end
