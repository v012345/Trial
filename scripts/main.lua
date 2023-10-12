xpcall(function()
    print("Hello 👋")
    require "NimotsuKun"
end, function(msg)
    print(msg)
end)
