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
