package.path = package.path .. ";./?.lua"
xpcall(function()
    os.execute("chcp 65001 > NUL")
    print("Hello 👋")
    local lfs = require "lfs"
    -- local currentdir = lfs.currentdir()
    local currentdir = "D:\\Closers.cocos\\resource\\ui\\branches\\dzogame_sea\\zhcn"
    -- local transFileName = "trans.csv"
    local transFileName = "trans-test.csv"


    for i, v in ipairs(arg) do
        print(i, v)
    end
    if arg[3] == "extract" then
        print("<<<< 开始提取文字 >>>>")
        require "Tools.ParseXML"
        require "Tools.CSV"
        local trans = CSV()
        -- 5个语言全部加载进来
        local all_csd_path = {}
        local folder = currentdir .. "\\cocosstudio\\ui"
        for entry in lfs.dir(folder) do
            if entry ~= "." and entry ~= ".." then
                local filePath = folder .. "\\" .. entry
                local fileAttributes = lfs.attributes(filePath)
                if fileAttributes.mode == "file" then
                    if string.match(string.lower(filePath), "^.+%.csd$") then
                        all_csd_path[entry] = {
                            zhcn = filePath,
                            vi = string.gsub(filePath, "\\zhcn\\", "\\vi\\", 1),
                            th = string.gsub(filePath, "\\zhcn\\", "\\th\\", 1),
                            id = string.gsub(filePath, "\\zhcn\\", "\\id\\", 1),
                            en = string.gsub(filePath, "\\zhcn\\", "\\en\\", 1),
                        }
                    end
                end
            end
        end
        local tableHead = { "zhcn", "vi", "th", "id", "en", "csd", "name", "tag" }
        for i, v in ipairs(tableHead) do
            trans:setCell(1, i, v)
        end
        local row_number = 2

        ---comment
        ---@param csdName any
        ---@param zhcnCsd XMLNode
        ---@param viCsd XMLNode
        ---@param thCsd XMLNode
        ---@param idCsd XMLNode
        ---@param enCsd XMLNode
        local function extract(csdName, zhcnCsd, viCsd, thCsd, idCsd, enCsd)
            local attributes = { "ButtonText", "LabelText", "PlaceHolderText" }
            for _, attri in ipairs(attributes) do
                if zhcnCsd:getAttributeValue(attri) then
                    trans:setCell(row_number, 1, zhcnCsd:getAttributeValue(attri))
                    trans:setCell(row_number, 2, viCsd:getAttributeValue(attri))
                    trans:setCell(row_number, 3, thCsd:getAttributeValue(attri))
                    trans:setCell(row_number, 4, idCsd:getAttributeValue(attri))
                    trans:setCell(row_number, 5, enCsd:getAttributeValue(attri))
                    trans:setCell(row_number, 6, csdName)
                    trans:setCell(row_number, 7, enCsd:getAttributeValue("Name"))
                    trans:setCell(row_number, 8, enCsd:getAttributeValue("Tag"))
                    row_number = row_number + 1
                end
            end

            --- 文件结构全部一样
            for idx, zhcnChild in ipairs(zhcnCsd:getChildren()) do
                local viChild = viCsd:getChild(idx)
                local thChild = thCsd:getChild(idx)
                local idChild = idCsd:getChild(idx)
                local enChild = enCsd:getChild(idx)
                extract(csdName, zhcnChild, viChild, thChild, idChild, enChild)
            end
        end

        local index = 1
        for csdName, paths in pairs(all_csd_path) do
            print(index, csdName)
            index = index + 1
            local zhcnCsd = (ParseXML(paths.zhcn)):getData()
            local viCsd = (ParseXML(paths.vi)):getData()
            local thCsd = (ParseXML(paths.th)):getData()
            local idCsd = (ParseXML(paths.id)):getData()
            local enCsd = (ParseXML(paths.en)):getData()
            extract(csdName, zhcnCsd, viCsd, thCsd, idCsd, enCsd)
        end
        trans:writeTo(currentdir .. "\\" .. transFileName)
        print("<<<< 完成提取文字 >>>>")
    elseif arg[3] == "publish" then
        print("<<<< 开始发布变动 csd >>>>")
        print("<<<< 完成发布变动 csd >>>>")
    elseif arg[3] == "publishAll" then
        print("<<<< 开始发布全部 csd >>>>")
        print("<<<< 完成发布全部 csd >>>>")
    elseif arg[3] == "plist" then
        print("<<<< 开始发布 plist >>>>")
        print("<<<< 完成发布 plist >>>>")
    elseif arg[3] == "merge" then
        print("<<<< 开始合并翻译 >>>>")
        print("<<<< 完成合并翻译 >>>>")
    end
    os.execute("pause")
end, function(msg)
    print(msg)
    os.execute("pause")
end)
