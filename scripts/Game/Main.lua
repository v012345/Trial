xpcall(function()
    local t = Framework:createTexture(CMAKE_SOURCE_DIR .. "res/background.png")
    function MainLoop()
        xpcall(function()
            Framework:setTexture(t);
            Framework:drawTriangle2D(
                { 100, 100 },
                { 200.0, 120.0 },
                { 120.0, 200.0 },
                { 0.0, 0.0 },
                { 1.0, 0.0 },
                { 0.0, 1.0 },
                0xffffff00, 0xff00ffff, 0xffff00ff
            )
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
