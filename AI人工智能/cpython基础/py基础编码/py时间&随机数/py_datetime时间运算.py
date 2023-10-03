#coding=UTF-8



import datetime
import time



#
# 1.datetime.datetime 模块
#
# 获取当前日期&时间
print(datetime.datetime.now())

t = datetime.datetime.now()
print(t.strftime("%H:%M:%S"))
print(t.strftime("%m/%d/%Y, %H:%M:%S"))
print(t.strftime("%d/%m/%Y, %H:%M:%S"))


# 拆分年月日, 时分秒
t=datetime.datetime(2019, 12, 28, 23, 55, 59, 342380)
t2=datetime.datetime.now()
print(t.year)
print(t.month)
print(t.day)
print(t.hour)
print(t.minute)
print(t.second)


# 返回两个日期的时间差(可以有负数, 返回结果为timedelta 对象, 仍然可以打印, 但有些区别)
t3=t2-t
print(t3)
t4=t-t2
print(t4)

# 拆分days, seconds
print(t4.days)
print(t4.seconds)
print(t4.microseconds)
#print(dir(t4))

# 返回时间差的总秒数(单位: 秒.微秒)
print(t4.total_seconds())

# 总秒数换回日期(错误想法, 两个时间相减之后的结果, 不能换回日期, 相减后的结果再也不是日期, 而是日期差)
#print(datetime.date.fromtimestamp(t4.total_seconds()))





#
# 2.datetime.date 模块
#
# 获取当前日期
print(datetime.date.today())

# 拆分年月日
t=datetime.date.today()
print(t.year)
print(t.month)
print(t.day)



# 将时间戳变成日期
print(datetime.date.fromtimestamp(1576244364))
print(datetime.date.fromtimestamp(time.time()))





#
# 3.datetime.time 模块
#
# time(hour = 0, minute = 0, second = 0)
print(datetime.time())

# time(hour, minute, second)
print(datetime.time(11, 34, 56))
print(datetime.time(hour = 11, minute = 34, second = 56))

# time(hour, minute, second, microsecond微秒)
print(datetime.time(11, 34, 56, 234566))


# 拆分时间
t = datetime.time(11, 34, 56, 234566)
print("hour =", t.hour)
print("minute =", t.minute)
print("second =", t.second)
print("microsecond =", t.microsecond)

