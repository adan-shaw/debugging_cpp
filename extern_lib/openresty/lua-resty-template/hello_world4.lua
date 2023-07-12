local template = require "resty.template"

local html = template.compile("lua_home/my_test/views/hello_world.html")({ message = 'Hello, World!' })
ngx.print(html)
