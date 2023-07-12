--两个关键变量宏
--[[
	template.root:
		等价于nginx.conf 里面的server{} location{} root, 用于指定location{} 块的root 根;
		这个作用范围比template.location 小, 最大等于一整个location{}, 也可能是location{} 下面的一个小location{};
		(推荐使用template.root, 相当于在location{} 设置root 关键字)

	template.location:
		等价于nginx.conf 里面的server{} location{}, 比较少用;
		location{} 一般由nginx.conf 指定比较适合, resty.template 模块不能完全取代nginx.conf;

	安装了resty.template 模板之后, 你也可以在nginx.conf 中, 添加:
		#set $template_root /home/tarball/openresty/html;
		#set $template_location /html;
]]



--resty.template 模板API
--[[
	render_string				function: 0x7f67f6e0d468
	escape							function: 0x7f67f6e3f030
	load								function: 0x7f67f6e0e0b0
	render							function: 0x7f67f6e0d3e8
	process							function: 0x7f67f6e0d328
	print								function: 0x7f67f6e026e0
	_VERSION						2.0
	compile							function: 0x7f67f6e3f1a0
	render_file					function: 0x7f67f6e0d428
	precompile_string		function: 0x7f67f6e3f130
	precompile_file			function: 0x7f67f6e3f168
	parse_file					function: 0x7f67f6e3f298
	parse_string				function: 0x7f67f6e0d2f0
	new									function: 0x7f67f6e3f088
	load_file						function: 0x7f67f6e0e178
	load_string					function: 0x7f67f6e0e1e8
	compile_string			function: 0x7f67f6e3f260
	visit								function: 0x7f67f6e0e2e8
	process_file				function: 0x7f67f6e0d368
	output							function: 0x7f67f6e3efa0
	process_string			function: 0x7f67f6e0d3a8
	parse								function: 0x7f67f6e0d218
	cache{}							空table
	compile_file				function: 0x7f67f6e3f228
	caching							function: 0x7f67f6e0e358
	precompile					function: 0x7f67f6df8080
]]
