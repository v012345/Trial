-- xpcall(function()
--     print("Hello 👋")
--     -- require "NimotsuKun"
--     require "RemoveBOM"
--     end, function(msg)
--     print(msg)
-- end)
for i = 1, 10, 1 do
    for j = 1, 10, 1 do
        Impl:vram(i, j, 0xFF0000)
    end
end
