local template = require "resty.template"

-- Using template.new
local view = template.new("lua_home/my_test/views/easy内嵌-aside.html", "lua_home/my_test/views/easy内嵌.html")
view.title = "easy内嵌.html - title"
view.message  = "easy内嵌div.html - message"
view.keyword  = "easy内嵌div.html - keyword"

local html = tostring(view)
ngx.print(html)



--使用template.new 的特点:
--[[
	Using template.new 组建字符串, 不使用template:render(), 
	而是生成一个template 容器, 负责组建html 字符串, 
	组建完成之后, 直接tostring()转变成字符串, 再使用ngx.print()将结果打印到标准output, 再根据fastCGI 返回给web客户端;

	这种方法消耗很大, 将template.new() 视图转为字符串, 是一种内耗;
	再次调用ngx.print(), 类似template:render(), 消耗很大, 不推荐用;

	推荐一个html 包含了所有, 这样就最简单, 如果怕被盗, 可以将关键css, js, 单独下发(具体防盗手段, 自己想)
]]
