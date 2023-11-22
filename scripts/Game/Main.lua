xpcall(function()
    function MainLoop()
        xpcall(function()
            Framework:drawTriangle2D(
                { 100, 100 },
                { 50, 200 },
                { 150, 200 },
                0, 0, 0,
                0xffff0000, 0xff00ff00, 0xff0000ff)
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
