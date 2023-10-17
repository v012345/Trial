xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    -- print(.height)
    local p = ReadPngFile(CMAKE_SOURCE_DIR .. "/res/shengyue.png")
    -- for index, value in ipairs(p.RGB) do
    --     print(index, string.format("%x", value))
    -- end
    -- print(p.height, p.width, #p.RGB)
    -- for i = 1, p.height, 1 do
    --     for j = 1, p.width, 1 do
    --         io.write(string.format("%x", p.RGB[(i - 1) * p.width + j]), "\t")
    --     end
    --     io.write("\n")
    -- end

    print("Hello1 👋")
    function MainLoop()
        for i = 1, p.height, 1 do
            for j = 1, p.width, 1 do
                Impl:vram(i - 1, j - 1, p.RGB[(i - 1) * p.width + j])
            end
        end
    end
end, function(msg)
    print(msg)
end)
