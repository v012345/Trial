xpcall(function(...)
    require "get"
    local socket = require("socket")
    local http = require("socket.http")
    local listening_port = 8080
    -- 创建服务器
    local server = socket.bind("*", listening_port)
    if not server then
        print("Can't run server")
        return
    end
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
        if request == "update-self" then
            client:send(response)
            client:close()
            local file = io.open("update.lua", "w")
            if file then
                getbyhttp("https://raw.githubusercontent.com/v012345/Trial/master/server/update.lua", file)
            end
            print("Client disconnected")
            print("Close server")
            return
        elseif request == "update" then
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
