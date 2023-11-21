xpcall(function()
    function MainLoop()
        xpcall(function()
            Framework:drawTriangle2D(
                { 20, 1 },
                { 20, 100 },
                { 100, 100 }
            )
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
