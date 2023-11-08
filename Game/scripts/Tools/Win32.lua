Win32 = {}
function Win32:getSha1(path)
    local windows_path = string.gsub(path, "/", "\\")
    local cmd = string.format('certutil -hashfile "%s"', windows_path)
    local result = io.popen(cmd) or error("can't run " .. cmd)
    local _, r = result:read("l"), result:read("l")
    result:close()
    return r
end

function Win32:copy(from, to)
    local windows_from = string.gsub(from, "/", "\\")
    local windows_to = string.gsub(to, "/", "\\")
    local cmd = string.format('copy "%s" "%s" /Y', windows_from, windows_to)
    local r = io.popen(cmd)
    if not r then
        error("can't run " .. cmd)
    else
        print(r:read("a"))
        r:close()
    end
end
