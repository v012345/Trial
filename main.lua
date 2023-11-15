-- local socket = require "socket"
-- local mime   = require "mime"
-- print("Hello from " .. socket._VERSION .. " and " .. mime._VERSION .. "!")
os.execute(string.format("py -3 %s -I %s -O %s", CMAKE_SOURCE_DIR .. "python/CovertExcelToCsv.py",
    "D:/Closers.cocos/resource/data/branches/dzogame_sea/excel/action_attack_effect.xls",
    CMAKE_SOURCE_DIR .. "/log/text.csv"))
