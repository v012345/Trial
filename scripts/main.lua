-- xpcall(function()
--     print("Hello 👋")
--     -- require "NimotsuKun"
--     require "RemoveBOM"
--     end, function(msg)
--     print(msg)
-- end)

local file = io.open(CMAKE_SOURCE_DIR .. "/log/log.txt", "w") or error()
file:write("ll22222222222llll");
file:close()

print("lua:jifjisojfseifjsijfii")
print("lua:", Impl, type(getmetatable(Impl)), Impl.height, Impl:height())

for i = 1, 10, 1 do
    for j = 1, 10, 1 do
        Impl:vram(i, j, 0xFF0000)
    end
end
