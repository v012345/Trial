local lfs = require "lfs"
local allFiles = {}
local function getAllFiles(dir)
    for entry in lfs.dir(dir) do
        if entry ~= "." and entry ~= ".." and entry ~= "build" and entry ~= ".git" and entry ~= ".vscode" then
            local filePath = dir .. "/" .. entry
            local fileAttributes = lfs.attributes(filePath)
            if fileAttributes.mode == "directory" then
                getAllFiles(filePath)
            elseif fileAttributes.mode == "file" then
                allFiles[#allFiles + 1] = filePath
            end
        end
    end
end
getAllFiles(CMAKE_SOURCE_DIR)
print(#allFiles)
for _, file in ipairs(allFiles) do
    local f = io.open(file, "r") or error()
    local c = f:read("a")
    f:close()
    if #c > 3 then
        local bom = string.format("%x%x%x", string.byte(c, 1, 3))
        if string.lower(bom) == "efbbbf" then
            c = string.sub(c, 4, #c)
            f = io.open(file, "w") or error()
            f:write(c)
            f:close()
            print(file)
        end
    end
end
