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


-- xpcall(function(...)
--     require "Tools.JSON"
--     local socket = require("socket")

--     -- local host = "2.tcp.cpolar.cn"
--     -- local port = 14731

--     local host = "192.168.1.6"
--     -- local port = 3389

--     -- local host = "127.0.0.1"
--     local port = 8080

--     -- 创建 TCP 客户端
--     local client = socket.tcp()

--     -- 连接到服务器
--     client:connect(host, port)
--     -- tasklist /fi "imagename eq nginx.exe"
--     -- 发送 HTTP GET 请求
--     local c = {}
--     c.cmd = "execute"
--     -- c.cmd = "update"
--     -- c.cmd = "update-self"
--     c.source = "http://192.168.1.15:8080/server/update.exe"
--     c.target = "update.exe"
    
--     -- c.chunk = "local x = {...}; print(x[1]);x[1]:send('123\\n');local a = io.open(\"update_restart.bat\",'w');a:write('taskkill /fi \"imagename eq update.exe\"\\nstart update.exe');a:close() "
--     c.chunk = "call_restart_bat()"
--     local request = JSON:convert(c) .. "\n"
--     print("111")
--     client:send(request)
--     print("222")

--     -- 读取服务器响应
--     local response, err = client:receive("*a")

--     if response then
--         print("Response body:")
--         print(response)
--     else
--         print("Error:", err)
--     end

--     -- 关闭连接
--     client:close()
-- end, function(m)
--     print(m)
-- end)
