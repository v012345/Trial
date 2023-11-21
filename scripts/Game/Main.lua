xpcall(function()
    require "Math.Vector2"
    require "Math.Matrix22"
    require "Math.Matrix23"
    local gImage = Image(CMAKE_SOURCE_DIR .. "res/background.png")
    local gCount = 0
    local function rotate(out, input, offset, matrix)
        local tmpOffset = Vector2(-0.5, -0.5);
        tmpOffset = tmpOffset + offset;
        out:setSub(input, tmpOffset);
        matrix:multiply(out, out);
        out = out + tmpOffset;
    end
    local function rasterize(a, b, c, ai, bi, ci, image)
        local p0
        local p1
        local p2
        local i0
        local i1
        local i2
        if a.y < b.y then
            if b.y < c.y then
                p0 = a;
                p1 = b;
                p2 = c;
                i0 = ai;
                i1 = bi;
                i2 = ci;
            elseif a.y < c.y then
                p0 = a;
                p1 = c;
                p2 = b;
                i0 = ai;
                i1 = ci;
                i2 = bi;
            else
                p0 = c;
                p1 = a;
                p2 = b;
                i0 = ci;
                i1 = ai;
                i2 = bi;
            end
        else
            if a.y < c.y then
                p0 = b;
                p1 = a;
                p2 = c;
                i0 = bi;
                i1 = ai;
                i2 = ci;
            elseif b.y < c.y then
                p0 = b;
                p1 = c;
                p2 = a;
                i0 = bi;
                i1 = ci;
                i2 = ai;
            else
                p0 = c;
                p1 = b;
                p2 = a;
                i0 = ci;
                i1 = bi;
                i2 = ai;
            end
        end
        local p01 = Vector2()
        local p02 = Vector2()
        local p03 = Vector2()

        p01:setSub(p1, p0);
        p02:setSub(p2, p0);
        local p3 = Vector2()

        local t = (p1.y - p0.y) / p02.y;
        p3.x = p0.x + p02.x * t;
        p3.y = p1.y;

        p03:setSub(p3, p0);

        -- // 图像侧的下标与屏幕上的位置之间的关系由等式表示。
        -- // 要求出x和y，就必须知道ix和iy。
        local a0, a1, a2, a3, a4, a5
        local i01 = Vector2()
        local i02 = Vector2()

        i01:setSub(i1, i0);
        i02:setSub(i2, i0);
        -- // 一个映射用的矩阵, 从可视三角形中上点, 映射回纹理三角形上
        -- // 即 P = AI, 因为两个三角形为已知量, 使用待定系数法, 可以解出下面 6 个值
        a0 = (i01.x * p02.y - i02.x * p01.y) / (p01.x * p02.y - p02.x * p01.y);
        a1 = (i01.x * p02.x - i02.x * p01.x) / (p01.y * p02.x - p02.y * p01.x);
        a2 = i0.x - p0.x * a0 - p0.y * a1;
        a3 = (i01.y * p02.y - i02.y * p01.y) / (p01.x * p02.y - p02.x * p01.y);
        a4 = (i01.y * p02.x - i02.y * p01.x) / (p01.y * p02.x - p02.y * p01.x);
        a5 = i0.y - p0.x * a3 - p0.y * a4;

        -- // 从上半三角形开始。p0，p1，p3。
        -- // 首先，将Y的范围设为整数。
        if p1.y > p0.y + 1.0 then
            local yBegin = math.floor(p0.y + 0.5)
            local yEnd = math.floor(p1.y + 0.5)

            local rcpP01y = 1.0 / p01.y;
            for y = yBegin, yEnd do
                -- // 检查指定y处x的范围。
                local yf = y;
                -- // 只需找到p3-p0和p1-p0的两边与y的当前水平线的交点即可。
                -- // 计算与以前相同。
                t = (y - p0.y) * rcpP01y;
                local x0 = p0.x + p01.x * t; --// p0.x + p01.x * ( y - p0.y ) / p01.y
                local x1 = p0.x + p03.x * t; --// p0.x + p01.x *（y-p0.y）/ p03.y不过p01.y = p03.y
                local xBegin, xEnd;
                if x0 > x1 then
                    xBegin = math.floor(x1 + 0.5)
                    xEnd = math.floor(x0 + 0.5)
                else
                    xBegin = math.floor(x0 + 0.5)
                    xEnd = math.floor(x1 + 0.5)
                end
                for x = xBegin, xEnd do
                    local xf = x;
                    -- // 从x，y中找到ix，iy
                    local ip = Vector2()
                    ip.x = a0 * xf + a1 * yf + a2;
                    ip.y = a3 * xf + a4 * yf + a5;
                    -- // 获取像素
                    -- vram[y * ww + x] = image->pixel(ip);
                    Framework:setVideoMemory(x, y, gImage:pixel(math.floor(ip.x + 0.5), math.floor(ip.y + 0.5)))
                end
            end
        end
        -- // 接下来是下半三角形。完成的工作几乎是相同的。
        if p2.y > p1.y + 1.0 then
            local p21 = Vector2()
            local p23 = Vector2()

            p21:setSub(p1, p2);
            p23:setSub(p3, p2);
            local yBegin = math.floor(p1.y + 0.5)
            local yEnd = math.floor(p2.y + 0.5)

            local rcpP21y = 1.0 / p21.y;
            for y = yBegin, yEnd do
                -- // 检查指定y处x的范围。
                local yf = y;
                -- // 只需找到p3-p2和p1-p2的两边与y的当前水平线的交点即可。
                -- // 计算与以前相同。
                t = (y - p2.y) * rcpP21y;
                local x0 = p2.x + p21.x * t; --// p2.x + p21.x * ( y - p2.y ) / p21.y
                local x1 = p2.x + p23.x * t; --// p2.x + p21.x *（y-p2.y）/ p23.y其中p21.y = p23.y.
                local xBegin, xEnd;
                if x0 > x1 then
                    xBegin = math.floor(x1 + 0.5)
                    xEnd = math.floor(x0 + 0.5)
                else
                    xBegin = math.floor(x0 + 0.5)
                    xEnd = math.floor(x1 + 0.5)
                end
                for x = xBegin, xEnd do
                    local xf = x;
                    -- // 从x，y中找到ix，iy
                    local ip = Vector2()
                    -- Vector2 ip;
                    ip.x = a0 * xf + a1 * yf + a2;
                    ip.y = a3 * xf + a4 * yf + a5;
                    -- // 获取像素
                    -- vram[y * ww + x] = image->pixel(ip);
                    Framework:setVideoMemory(x, y, gImage:pixel(math.floor(ip.x + 0.5), math.floor(ip.y + 0.5)))
                end
            end
        end
    end
    print(math.atan(1, 1) * 180 / math.pi)
    print(math.atan(1, -1) * 180 / math.pi)
    print(math.atan(-1, -1) * 180 / math.pi)
    print(math.atan(-1, 1) * 180 / math.pi)
    function MainLoop()
        xpcall(function()
            local ww = Framework:width()
            local wh = Framework:height()
            for y = 0, wh - 1 do
                for x = 0, ww - 1 do
                    Framework:setVideoMemory(x, y, 0)
                end
            end
            local iw = gImage:width();
            local ih = gImage:height();
            local offset = Vector2()
            offset.x = (ww) / 2.0;
            offset.y = (wh) / 2.0;
            local rotation = (gCount);
            local sine = math.sin(rotation * math.pi / 180);
            local cosine = math.cos(rotation * math.pi / 180);
            local matrix = Matrix22(cosine, -sine, sine, cosine);
            -- local matrix = Matrix22(sine + 0.5, 0, 0, sine + 0.5);

            local a = Vector2()
            local b = Vector2()
            local c = Vector2()
            local d = Vector2()
            rotate(a, Vector2((ww - iw) / 2, (wh - ih) / 2), offset, matrix);
            rotate(b, Vector2((ww + iw) / 2 - 1, (wh - ih) / 2), offset, matrix);
            rotate(c, Vector2((ww - iw) / 2, (wh + ih) / 2 - 1), offset, matrix);
            local ab = Vector2()
            local ac = Vector2()

            ab:setSub(b, a);
            ac:setSub(c, a);
            d:setAdd(a, ab);
            d = d + ac;
            --// 计算a，b，c，d中图像侧的下标值
            local ai = Vector2(0, 0);
            local bi = Vector2(iw - 1, 0);
            local ci = Vector2(0, ih - 1);
            local di = Vector2(bi.x, ci.y);
            -- // 开始栅格化
            rasterize(a, b, c, ai, bi, ci, gImage); --// 三角形abc
            rasterize(d, b, c, di, bi, ci, gImage); --// 三角形dbc
            gCount = gCount + 1
            Framework:drawDebugString(70, 0, "FPS : " .. Framework:frameRate())
        end, function(msg)
            print(msg)
        end)
    end
end, function(msg)
    print(msg)
end)
