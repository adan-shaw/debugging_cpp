#coding=UTF-8



from http.server import SimpleHTTPRequestHandler, HTTPServer
import ssl

# 创建服务器对象
httpd = HTTPServer(('127.0.0.1', 8000), SimpleHTTPRequestHandler)

# 添加SSL/TLS层(需要自己生成./CA.key, ./CA.crt, 作者自己的证书不方便提供)
httpd.socket = ssl.wrap_socket(httpd.socket,
                               keyfile="./CA.key",
                               certfile='./CA.crt',
                               server_side=True)

print("Serving on port https://localhost:8000 with https support")
# 启动服务器(阻塞)
httpd.serve_forever()



# 测试
#curl 127.0.0.1:8000
