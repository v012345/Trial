xpcall(function()
    require "Game.Enum"
    -- local t = Framework:createTexture(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0;
    Framework:setFrameRate(60)
    local A = Enum.BlendMode.BLEND_OPAQUE
    local a = "OPAQUE"
    local B = Enum.BlendMode.BLEND_LINEAR
    local b = "LINEAR"
    function MainLoop()
        xpcall(function()
            Framework:drawDebugString(0, 0, "press A or D to change blend mode.");
            Framework:drawDebugString(0, 1, "current blend mode of A : " .. a);
            Framework:drawDebugString(0, 2, "current blend mode of D : " .. b);
            -- Framework:setTexture(t);
            Framework:setBlendMode(A)
            Framework:drawTriangle2D(
                { 100.0, 100.0 },
                { 200.0, 120.0 },
                { 120.0, 200.0 },
                0, 0, 0, 0xffff8080, 0xff80ff80, 0xff8080ff
            )
            local alpha = (gCount % 256) << 24;
            gCount = gCount + 1
            Framework:setBlendMode(B)
            Framework:drawTriangle2D(
                { 110.0, 110.0 },
                { 210.0, 130.0 },
                { 130.0, 210.0 },
                0, 0, 0,
                0x80ff80| alpha, 0x8080ff| alpha, 0xff8080| alpha
            )
            if Framework:isKeyTriggered(Enum.Keyboard.D) then
                if B == Enum.BlendMode.BLEND_LINEAR then
                    B = Enum.BlendMode.BLEND_ADDITIVE
                    b = "ADDITIVE"
                elseif B == Enum.BlendMode.BLEND_ADDITIVE then
                    B = Enum.BlendMode.BLEND_OPAQUE
                    b = "OPAQUE"
                elseif B == Enum.BlendMode.BLEND_OPAQUE then
                    B = Enum.BlendMode.BLEND_LINEAR
                    b = "LINEAR"
                end
            end
            if Framework:isKeyTriggered(Enum.Keyboard.A) then
                if A == Enum.BlendMode.BLEND_LINEAR then
                    A = Enum.BlendMode.BLEND_ADDITIVE
                    a = "ADDITIVE"
                elseif A == Enum.BlendMode.BLEND_ADDITIVE then
                    A = Enum.BlendMode.BLEND_OPAQUE
                    a = "OPAQUE"
                elseif A == Enum.BlendMode.BLEND_OPAQUE then
                    A = Enum.BlendMode.BLEND_LINEAR
                    a = "LINEAR"
                end
            end
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
