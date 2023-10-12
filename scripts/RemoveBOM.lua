local lfs = require "lfs"
print(lfs.currentdir())
-- print(CMAKE_SOURCE_DIR)
print(lfs.chdir(CMAKE_SOURCE_DIR))
print(lfs.currentdir())
-- lfs.chdir(CMAKE_SOURCE_DIR)
local function getAllFiles(dir)
    for entry in lfs.dir(dir) do
        if entry ~= "." and entry ~= ".." and entry ~= "build" then
            local filePath = dir .. "/" .. entry
            local fileAttributes = lfs.attributes(filePath)
            if fileAttributes.mode == "directory" then
                traverseDirectory(filePath, root, prefix)
            elseif fileAttributes.mode == "file" then

            end
        end
    end
end
getAllFiles(CMAKE_SOURCE_DIR)
