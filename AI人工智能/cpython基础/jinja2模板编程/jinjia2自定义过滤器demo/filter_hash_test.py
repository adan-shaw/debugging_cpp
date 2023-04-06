#coding=utf-8



import jinja2
from filter_hash import jinja2_filter_hash

# 装载jinja2 环境
env = jinja2.Environment()

# 新增jinja2_filter 自定义过滤器函数
env.filters["hash"] = jinja2_filter_hash

# 定义jinja2 '模板文件/模板str'[文件即字符串, 字符串即文件]
tmpl_string = """MD5 hash of '$3cr3tP44$$': {{ '$3cr3tP44$$' | hash('md5') }}"""

# 装载'模板文件/模板str'到env 环境中
tmpl = env.from_string(tmpl_string)

# 执行jinja2 渲染
print(tmpl.render())
