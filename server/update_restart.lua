xpcall(function(...)
    require "get"
    local socket = require("socket")
    local listening_port = 8081
    -- 创建服务器
    local server = socket.bind("*", listening_port)
    if not server then
        print("Can't run server")
        return
    end
    local x = io.popen('tasklist /fi "imagename eq update.exe"') or error("")
    local r = x:read("a")
    print(r)
    if string.match(r, "update.exe") then
        print("kkk")
        local x = io.popen('taskkill /f /fi "imagename eq update.exe"')
        local r = x:read("a")
        print(os.execute("start update.exe"))
        -- print(r)
    else
        print(os.execute("start update.exe"))
    end
    print(r)
    print("Server is listening on port " .. listening_port)
    print("===============================")
    while true do
        -- 等待客户端连接
        local client = server:accept()
        print("Client connected")
        local response = "ok"
        -- 读取客户端请求
        local request, err = client:receive()
        print(request)
        ---@type table
        ---@diagnostic disable-next-line
        local r = JSON(request):getData()
        if r.cmd == "update-self" then
            client:send(response)
            client:close()
            get(r.source, r.target)
            print("Client disconnected")
            print("Close server")
            return
        elseif r.cmd == "update" then
            get(r.source, r.target)
        elseif r.cmd == "execute" then
            xpcall(function(...)
                load(r.chunk)(client)
            end, function(msg)
                print(msg)
            end)
        end

        if not err then
            -- 处理请求并发送响应
            client:send(response)
        end

        -- 关闭客户端连接
        client:close()
        print("Client disconnected")
        print("===============================")
    end
end, function(msg)
    print(msg)
end)
