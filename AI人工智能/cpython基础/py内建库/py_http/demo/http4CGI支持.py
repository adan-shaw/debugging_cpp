#coding=UTF-8



from http.server import HTTPServer, CGIHTTPRequestHandler

# 设置服务器地址和端口
server_address = ('', 8000)

# 启用CGI处理程序
httpd = HTTPServer(server_address, CGIHTTPRequestHandler)

print("Serving on port 127.0.0.1:8000 with CGI support")
# 启动服务器(阻塞)
httpd.serve_forever()



# 测试
#curl 127.0.0.1:8000
