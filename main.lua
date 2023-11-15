xpcall(function(...)
    -- load namespace
    local socket = require("socket")
    -- create a TCP socket and bind it to the local host, at any port
    local server = assert(socket.bind("127.0.0.1", 8848))
    -- find out which port the OS chose for us
    local ip, port = server:getsockname()
    -- print a message informing what's up
    print("Please telnet to localhost on port " .. port)
    print("After connecting, you have 10s to enter a line to be echoed")
    -- wait for a connection from any client
    print("lll")
    local client = server:accept()
    -- loop forever waiting for clients
    while 1 do
        print("kkk")
        -- make sure we don't block waiting for this client's line
        client:settimeout(10)
        -- receive the line
        local line, err = client:receive()
        -- if there was no error, send it back to the client
        if not err then client:send(line .. "\n") end
        -- done with client, close the object
    end
    client:close()
end, function(msg)
    print(msg)
end)
