local socket = require("socket")

-- 创建一个 TCP 服务器
local server = assert(socket.bind("*", 8849))
print("Server listening on port 8849")


while true do
    local client = server:accept()
    print("Client connected!")
    while true do
        -- 接收客户端发送的消息
        local line, err = client:receive()
        if not err then
            print("Received message from client: " .. line)
            -- 发送响应给客户端
            -- socket:sleep(1000)
            client:send("ok\n")
        else
            print("Client disconnected!")
            break
        end
    end
    client:close()
end
