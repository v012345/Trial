xpcall(function(...)
    require "Tools.HTML"
    require "Tools.Win32"
    local lfs = require "lfs"
    local function parse_cache()
        local cache_file = io.open(CMAKE_SOURCE_DIR .. "log/c.txt") or error("can't open c.txt")
        local cache = {}
        local URL = ""
        while true do
            local line = cache_file:read("l")
            if line then
                if string.match(line, "^URL               : ") then
                    URL = string.sub(line, 49, #line)
                elseif string.match(line, "^Cache Name        : ") then
                    local temp = string.sub(line, 21, #line)
                    temp = string.gsub(temp, " *", "")
                    temp = string.gsub(temp, "%[", "_")
                    temp = string.gsub(temp, "]", "")
                    cache[URL] = temp .. ".webp"
                end
            else
                break
            end
        end
        return cache
    end
    local cache = parse_cache()

    local h = HTML(CMAKE_SOURCE_DIR .. "log/h.html"):getData()
    local d = h:getChildren()
    local pages = {}
    for _, div in ipairs(d) do
        local id = div:getAttributeValue("id")
        pages[id] = {}
        for _, img in ipairs(div:getChildren()) do
            local style = img:getAttributeValue("style")
            local left_start, left_end = string.find(style, "left: ")
            local position = tonumber(string.sub(style, left_end + 1, #style - 3))
            if not position then
                error("position error")
            end
            local s = {
                src = img:getAttributeValue("src"),
                position = position
            }
            pages[id][#pages[id] + 1] = s
        end
        table.sort(pages[id], function(a, b)
            return a.position < b.position
        end)
    end
    local done_pages = {}
    for key, value1 in pairs(pages) do
        -- print(key)
        local done = true
        for index, value in ipairs(value1) do
            local page_sub_file_name = key .. "_" .. index .. ".webp"
            local page_sub_file = CMAKE_SOURCE_DIR .. "log/chromecacheview/pages/" .. page_sub_file_name
            local page_sub_file_is_exist = lfs.attributes(CMAKE_SOURCE_DIR ..
                "log/chromecacheview/pages/" .. page_sub_file_name)
            if not page_sub_file_is_exist then
                done = false
            end
            -- print(page_sub_file_name, page_sub_file)
            local cache_name = cache[value.src]
            if cache_name then
                done = true
                Win32:copy(CMAKE_SOURCE_DIR .. "log/chromecacheview/CacheFiles/" .. cache_name, page_sub_file)
            end
            -- print(value.position)
        end
        if done then
            done_pages[tonumber(key)] = true
        end
    end
    io.write("var has_done = {\n")
    for key, value in pairs(done_pages) do
        io.write(string.format('"%s" : true,\n', key))
    end
    io.write("\n};")
end, function(msg)
    print(msg)
end)
