xpcall(function(...)
    require "Tools.JSON"
    require "get"
    local socket = require("socket")
    local listening_port = 8081
    -- 创建服务器
    local server = socket.bind("*", listening_port)
    if not server then
        print("Can't run server")
        return
    end
    local function run_update_exe()
        if string.match(io.popen('tasklist /fi "imagename eq update.exe"'):read("a"), "update.exe") then
            io.popen('taskkill /f /fi "imagename eq update.exe"'):read("a")
        end
        os.execute("start update.exe")
    end
    run_update_exe()

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
        if r.cmd == "restart_update_exe" then
            run_update_exe()
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
