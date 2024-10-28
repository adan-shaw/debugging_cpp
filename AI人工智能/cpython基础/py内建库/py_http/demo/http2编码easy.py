#coding=UTF-8



from http.server import SimpleHTTPRequestHandler, HTTPServer

# 设置服务器地址和端口
server_address = ('127.0.0.1', 8000)

# 创建服务器对象
httpd = HTTPServer(server_address, SimpleHTTPRequestHandler)

print("Serving on port 127.0.0.1:8000")
# 启动服务器(阻塞)
httpd.serve_forever()



# 测试
#curl 127.0.0.1:8000
