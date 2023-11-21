xpcall(function()
    function MainLoop()
        xpcall(function()
            Framework:drawTriangle2D(
                { 10.0, 10.0 },
                { 200.0, 120.0 },
                { 120.0, 200.0 }
            )
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
