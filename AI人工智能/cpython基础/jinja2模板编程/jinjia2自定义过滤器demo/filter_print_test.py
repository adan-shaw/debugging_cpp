#coding=utf-8



import jinja2



# jinja2 自定义过滤器: 打印tuple 线性容器
def jinja2_filter_print_tuple(*args):
	plist=list(args)
	del plist[0]
	plist=tuple(plist)
	return str(plist)

# jinja2 自定义过滤器: 打印tuple 线性容器
def jinja2_filter_print_list(*args):
	plist=list(args)
	del plist[0]
	return str(plist)



# 声明:
# jinja2 模板不支持 **kwargs 键值对-多值参数函数, 仅支持*args 线性多值参数函数, 别问为什么!!
# 这样做的效率, 不一定比jinja2 自身逻辑遍历性能好, 例如:
'''
	ptuple = ( {% for v in ptuple %} {{v}}, {% endfor %} )
	plist = [ {% for v in plist %} {{v}}, {% endfor %} ]
	pdict = { {% for k,v in pdict.items() %} {{k}}-{{v}}, {% endfor %} }
'''
# 这里仅做jinja2 自定义过滤器demo 展示;



# 装载jinja2 环境
env = jinja2.Environment()

# 新增jinja2_filter 自定义过滤器函数
env.filters["print_tuple"] = jinja2_filter_print_tuple
env.filters["print_list"] = jinja2_filter_print_list



ptuple=("aa","bb","c")
plist=["aa","bb","c"]
pdict={"aa":1,"bb":2,"c":3}

tmpl_html_ptuple = """jinja2_filter_print_tuple(): {{ "" | print_tuple(*ptuple) }}"""
tmpl_html_plist = """jinja2_filter_print_list(): {{ "" | print_list(*plist) }}"""
tmpl_html_pdict = """jinja2_filter_print_dict(): { {% for k,v in pdict.items() %} {{k}}-{{v}}, {% endfor %} }"""



tmpl_tuple = env.from_string(tmpl_html_ptuple)
tmpl_list = env.from_string(tmpl_html_plist)
tmpl_dict = env.from_string(tmpl_html_pdict)



print(tmpl_tuple.render(ptuple=ptuple))
print(tmpl_list.render(plist=plist))
print(tmpl_dict.render(pdict=pdict))
