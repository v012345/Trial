-- xpcall(function(...)
--     -- require "get"
--     -- local file = io.open("2.txt", "w")
--     -- if file then
--     --     print('KKK')
--     --     file:write("22")
--     --     getbyhttp("http://localhost:8080/1.txt", file)

--     --     file:close()
--     -- end
--     require "get"
--     local file = io.open("update.lua", "w")
--     if file then
--         getbyhttp("https://raw.githubusercontent.com/v012345/Trial/master/server/update.lua", file)
--     end
-- end, function(msg)
--     print(msg)
-- end)
-- local json = require "Tools.JSON"
local socket = require("socket")

-- local host = "2.tcp.cpolar.cn"
-- local port = 14731

-- local host = "192.168.1.6"
-- local port = 3389

local host = "127.0.0.1"
local port = 8081

-- 创建 TCP 客户端
local client = socket.tcp()

-- 连接到服务器
client:connect(host, port)

-- 发送 HTTP GET 请求
local c = {}
c.cmd = "update"
c.source = "https://raw.githubusercontent.com/v012345/Trial/master/server/main.lua"
c.target = "main.lua"
local request = 'update-self\n'
client:send(request)

-- 读取服务器响应
local response, err = client:receive("*a")

if response then
    print("Response body:")
    print(response)
else
    print("Error:", err)
end

-- 关闭连接
client:close()
