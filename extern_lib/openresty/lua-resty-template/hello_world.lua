local template = require "resty.template"

--Using template.render_file() + file 直接提交渲染
--(注意: render_file() 方法会自动将结果打印到标准ouput; 根据fastCGI, 也就会自动返回给web 客户端)
template.render_file("lua_home/my_test/views/hello_world.html", { message = "Hello, World!" })



--template.render() 与 template:render() 的区别
--[[
	template.render() = 在这个实体的基础上, 依赖这个实体的数据, 再调用render();
	template:render() = 不依赖任何类实体的数据, 单纯调用公有类声明中的render();
]]



--render(), render_string(), render_file() 的区别:
--[[
	render():        自动识别是文件还是file, 默认为文件, 弃用, 不明确的东西不用;
	render_string(): 指定为string
	render_file():   指定为file
]]
