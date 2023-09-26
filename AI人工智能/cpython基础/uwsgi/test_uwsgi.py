#coding=utf8
def application(env, start_response):
	start_response('200 OK', [('Content-Type', 'text/html')])
	test = 'hello world'
	return test.encode("utf-8")

