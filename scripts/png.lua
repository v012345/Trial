local function png(path)
    local file = io.open(path, "rb") or error("can't open " .. path)
    local content = file:read("a")
    print(#content)
    for i = 1, 8, 1 do
        print(string.format("%x", string.byte(content, i, i)))
    end
    local s1 = 0
    for i = 4, 1, -1 do
        s1 = s1 + (string.byte(content, 8 + i, 8 + i) << (8 * (4 - i)))
    end
    print(s1)
    for i = 13, 16, 1 do
        print(i, string.char(string.byte(content, i, i)))
    end
end

return png
