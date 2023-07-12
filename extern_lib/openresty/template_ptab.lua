local template = require "resty.template"
--local template2 = require "resty.template.safe"
local ptab = require "print_tab".print_tab

ptab(template)
--ptab(template2)



--测试:
--./bin/resty -I ./lua_home/adan_lua ./lua_home/template_ptab.lua 



--证明: resty.template 是唯一的正确选择
--[[
	resty.template.safe 就是一个空table, 
	但并不是nil, resty.template.safe也是存在的, 只是以空table 的方式存在, 请勿使用;

	table: 0x7f67f6df7fb8{
		[render_string]=function: 0x7f67f6e0d468
		[escape]=function: 0x7f67f6e3f030
		[load]=function: 0x7f67f6e0e0b0
		[render]=function: 0x7f67f6e0d3e8
		[process]=function: 0x7f67f6e0d328
		[print]=function: 0x7f67f6e026e0
		[_VERSION]=2.0
		[compile]=function: 0x7f67f6e3f1a0
		[render_file]=function: 0x7f67f6e0d428
		[precompile_string]=function: 0x7f67f6e3f130
		[precompile_file]=function: 0x7f67f6e3f168
		[parse_file]=function: 0x7f67f6e3f298
		[parse_string]=function: 0x7f67f6e0d2f0
		[new]=function: 0x7f67f6e3f088
		[load_file]=function: 0x7f67f6e0e178
		[load_string]=function: 0x7f67f6e0e1e8
		[compile_string]=function: 0x7f67f6e3f260
		[visit]=function: 0x7f67f6e0e2e8
		[process_file]=function: 0x7f67f6e0d368
		[output]=function: 0x7f67f6e3efa0
		[process_string]=function: 0x7f67f6e0d3a8
		[parse]=function: 0x7f67f6e0d218
		[cache]{
		}
		[compile_file]=function: 0x7f67f6e3f228
		[caching]=function: 0x7f67f6e0e358
		[precompile]=function: 0x7f67f6df8080
	}
	table: 0x7f67f6e15598{
	}
]]

