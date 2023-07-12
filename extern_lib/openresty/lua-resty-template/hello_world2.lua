local template = require "resty.template"

--Using template.render_string() + string 直接提交渲染
--(注意: render_string() 方法会自动将结果打印到标准ouput; 根据fastCGI, 也就会自动返回给web 客户端)
template.render_string([[
<!DOCTYPE html>
<html>
<body>
  <h1>{{message}}</h1>
</body>
</html>]], { message = "Hello, World!" })



