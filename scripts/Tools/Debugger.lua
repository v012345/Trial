require "Tools.JSON"
local socket = require("socket")
local server = assert(socket.bind("127.0.0.1", 8849))
print("waiting debugger to connect to port 8849")
local debugger = server:accept()
local mySelf = debug.getinfo(1).short_src
print("debugger connected!")
debug.sethook(function(_, line_number)
    local s = debug.getinfo(2).short_src
    if s ~= mySelf then
        local j = JSON()
        local d = j:getData()
        d.file = s
        d.line = line_number
        debugger:send(j:toString())
        local msg_from_debugger, err = debugger:receive()
        if not err then
            print(msg_from_debugger)
            local info = debug.getinfo(2, "n") -- 获取上一层函数的信息
            if info then
                print("Local variables in function " .. (info.name or "unknown") .. ":")
                local index = 1
                while true do
                    local name, value = debug.getlocal(2, index)
                    if not name then break end
                    print(name, "=", value)
                    index = index + 1
                end
            else
                print("No information about the calling function.")
            end
        else
            print("debugger disconnected!")
            debugger:close()
            debug.sethook()
        end
    end
end, "l")
