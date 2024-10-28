#coding=UTF-8



from http.server import SimpleHTTPRequestHandler, HTTPServer

class MyRequestHandler(SimpleHTTPRequestHandler):
	def do_GET(self):
		if self.path == '/hello':
			self.send_response(200)
			self.send_header("Content-type", "text/html")
			self.end_headers()
			self.wfile.write(b"Hello, World!")
		else:
			super().do_GET()

# 创建服务器对象
httpd = HTTPServer(('127.0.0.1', 8000), MyRequestHandler)

print("Serving on port 127.0.0.1:8000")
# 启动服务器(阻塞)
httpd.serve_forever()



# 测试
#curl 127.0.0.1:8000
