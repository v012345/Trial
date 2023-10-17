xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    -- require "NimotsuKun"
    -- require "RemoveBOM"
    local png = require "png"
    png(CMAKE_SOURCE_DIR .. "/res/shengyue.png")
end, function(msg)
    print(msg)
end)
