xpcall(function()
    print("Hello 👋")
    -- require "NimotsuKun"
    require "RemoveBOM"
    end, function(msg)
    print(msg)
end)
