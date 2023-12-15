local function update_restart()
    require "Tools.JSON"
    local socket = require("socket")
    local host = "127.0.0.1"
    local port = 8081
    local client = socket.tcp()
    client:connect(host, port)
    local c = {
        cmd = "restart_update_exe"
    }
    local request = JSON:convert(c) .. "\n"
    client:send(request)
    local response = client:receive()
    client:close()
    print(response)
end

local function update_updateself()
    require "Tools.JSON"
    local socket = require("socket")
    local host = "127.0.0.1"
    local port = 8080
    local client = socket.tcp()
    client:connect(host, port)
    local c = {
        cmd = "update-self"
    }
    local request = JSON:convert(c) .. "\n"
    client:send(request)
    local response, err = client:receive()
    client:close()
    print(response)
end
local function update_file(source, target)
    require "Tools.JSON"
    local socket = require("socket")
    local host = "127.0.0.1"
    local port = 8080
    local client = socket.tcp()
    client:connect(host, port)
    local c = {
        cmd = "update",
        source = source,
        target = target,
    }
    local request = JSON:convert(c) .. "\n"
    client:send(request)
    local response, err = client:receive()
    client:close()
    print(response)
end
xpcall(function(...)
    -- update_file("http://raw.githubusercontent.com/v012345/Trial/master/server/update_restart.lua","update_restart.lua")
    -- update_updateself()
end, function(m)
    print(m)
end)
