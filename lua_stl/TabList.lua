--[[
说明:
	在TabQueue 的基础上, 改造出TabList, 
	元素少的情况下, 也可以适当频繁插入/删除(最好就不要频繁插入/删除)
--]]



local TabList = {}

TabList.__index = TabList

--创建队列(Type = number/string/anything)
--成功返回table, 失败返回nil
--[自己保证元素的一致性, 否则没办法排序]
function TabList:New( Type )
	local t = {}
	local Setmetatable = setmetatable
	if Type ~= "number" and Type ~= "string" and Type ~= "anything" then
		return nil
	end
	Setmetatable(t, self)
	t.__index = self
	t.Type = Type
	t.Pos = 0
	t.body = {}
	return t
end

--查看队头元素(不操作, 只查看)
--成功返回body元素, 失败返回nil
function TabList:Front()
	local Pos = self.Pos
	local Body = self.body
	return ((Pos <= 0)  and  nil  or  Body[1])
end

--查看队尾元素(不操作, 只查看)
--成功返回body元素, 失败返回nil
function TabList:Back()
	local Pos = self.Pos
	local Body = self.body
	return ((Pos <= 0)  and  nil  or  Body[Pos])
end

--从Back队尾插入
--成功返回true, 失败返回false(debug only)
function TabList:PushBack( val )
	local Pos = self.Pos
	local Body = self.body
	--debug 时开启, 关掉性能更好
	--[[
	local Type = self.Type
	local val_type = type(val)
	if val_type ~= Type and Type ~= "anything" then
		return false
	end
	--]]
	Pos = Pos + 1
	--更新Pos
	self.Pos = Pos
	Body[Pos] = val
	--return true
end

--从Back队尾弹出
--成功返回body元素, 失败返回nil
function TabList:PopBack()
	local Pos = self.Pos
	local Body = self.body
	if Pos <= 0 then
		return nil
	else
		Pos = Pos - 1
		--更新Pos
		self.Pos = Pos
		return Body[Pos + 1]
	end
end

--从Front队头插入
--成功返回true, 失败返回false(debug only)
function TabList:PushFront( val )
	local Insert = table.insert
	local Pos = self.Pos
	local Body = self.body
	--debug 时开启, 关掉性能更好
	--[[
	local Type = self.Type
	local val_type = type(val)
	if val_type ~= Type and Type ~= "anything" then
		return false
	end
	--]]
	--更新Pos
	self.Pos = Pos + 1
	Insert(Body, 1, val)
	--return true
end

--从Front队头弹出
--成功返回body元素, 失败返回nil
function TabList:PopFront()
	local Pos = self.Pos
	local ret = nil
	local Remove = table.remove
	local Body = self.body
	if Pos <= 0 then
		return nil
	else
		ret = Body[1]
		Remove(Body, 1)
		--更新Pos
		self.Pos = Pos - 1
		return ret
	end
end

--从Back队尾插入装有n 个元素的连续table(必须保证元素类型一致性, 遵守number/string/anything 原则)
function TabList:PushBack_TN( t )
	local Pos = self.Pos
	local Body = self.body
	--必须是'从下标1 开始的连续table', 否则#t 失败
	local tLen = #t
	local i = 1
	local pos = Pos
	for i=1,tLen,1 do
		pos = pos + 1
		Body[pos] = t[i]
	end
	--更新Pos
	self.Pos = pos
end

--从Front队头弹出装有n 个元素的连续table
--成功返回table, 失败返回nil
function TabList:PopFront_TN( n )
	local t1, t2 = {}, {}
	local Pos = self.Pos
	local Body = self.body
	local i = 1
	local tmp = 1
	--拒绝传入错误值, 要导出全部元素, 请先调用Len()
	if n > Pos or n <= 0 or Pos <= 0 then
		return nil
	end
	for i=1,n,1 do
		t1[i] = Body[i]
	end
	for i=n+1,Pos,1 do
		t2[tmp] = Body[i]
		tmp = tmp + 1
	end
	--self.body = nil
	self.body = t2
	self.Pos = Pos - n
	return t1
end

--栈空?
--成功返回true, 失败返回false
function TabList:Empty()
	local Pos = self.Pos
	return ((Pos <= 0)  and  true  or  false)
end

--返回栈当前元素的长度
function TabList:Len()
	local Pos = self.Pos
	return Pos
end

--清空(重置)
function TabList:Clear()
	self.body = {}
	self.Pos = 0
end

--重新排序, 打乱TabList 原来的顺序[先进先出TabList禁用](直接使用table.sort()进行排序)
function TabList:Sort()
	local Sort = table.sort
	local Body = self.body
	Sort(Body)
end

--洗牌乱序, 打乱TabList 原来的顺序, 随机排序
function TabList:Shuffle()
	local Pos = self.Pos
	local Body = self.body
	local Random = math.random
	local i, count = 1, Pos
	--n 个元素, 乱序n 次(不行可以增加乱序次数)
	--local i, count = 1, Pos*2
	local r1, r2 = 0, 0
	local tmp = nil
	for i=1,count,1 do
		r1 = Random(1, Pos)
		r2 = Random(1, Pos)
		--不管r1 是否等于r2, 坚决不用if, 直接交换, 相等也不碍事
		tmp = Body[r1]
		Body[r1] = Body[r2]
		Body[r2] = tmp
	end
end

--查找元素(不是pop, 只查找, 不删除)
--成功返回body元素, 失败返回nil
function TabList:Find( val )
	local Pos = self.Pos
	local Body = self.body
	local i = 0
	for i=1,Pos,1 do
		if Body[i] == val then
			--成功不是break跳出循环, 而是直接return返回, 言外之意是:
			--		当for循环耗尽, 还没批评到元素的时候, 一定是找不到元素, 返回nil
			return Body[i]
		end
	end
	return nil
end

--查找元素Pos
--成功返回body元素的Pos, 失败返回nil
function TabList:FindPos( val )
	local Pos = self.Pos
	local Body = self.body
	local i = 0
	for i=1,Pos,1 do
		if Body[i] == val then
			return i
		end
	end
	return nil
end

--根据pos 位置值, 插入元素(触发table重新排序)
function TabList:InsertPos( Pos, val )
	local Insert = table.insert
	local Body = self.body
	local Pos = self.Pos
	Insert(Body, Pos, val)
	self.Pos = Pos + 1
end

--根据pos 位置值, 删除元素(触发table重新排序)
--成功返回true, 失败返回false(debug only)
function TabList:Remove( val )
	local Remove = table.remove
	local Body = self.body
	local pos = self:FindPos(val)
	local Pos = self.Pos
	if pos ~= nil then
		Remove(Body, pos)
		self.Pos = Pos - 1
		--return true
	--else
		--return false
	end
end

--根据pos 位置值, 删除元素(触发table重新排序) [这个函数少用!! 不安全]
--成功返回true, 失败返回false(debug only)
function TabList:RemovePos( pos )
	local Remove = table.remove
	local Body = self.body
	local Pos = self.Pos
	if Pos >= pos and pos >= 1 then
		Remove(Body, pos)
		self.Pos = Pos - 1
		--return true
	--else
		--return false
	end
end

--自测函数(不对外公开)
local function Test(t_list_num)
	local i = 0
	local t0 = { 99, 88, 6, 7, 33, 1 }
	t_list_num:PushFront(19)
	t_list_num:PushBack(1)
	t_list_num:PushFront(1900)
	t_list_num:PushBack(222)
	t_list_num:InsertPos(2, 666)
	print("t_list_num.Pos\t=",t_list_num.Pos)
	for i=1,t_list_num.Pos,1 do
		print(t_list_num.body[i])
	end
	t_list_num:Shuffle()
	print("t_list_num.Pos\t=",t_list_num.Pos)
	for i=1,t_list_num.Pos,1 do
		print(t_list_num.body[i])
	end
	t_list_num:Sort()
	print("t_list_num.Pos\t=",t_list_num.Pos)
	for i=1,t_list_num.Pos,1 do
		print(t_list_num.body[i])
	end
	print(t_list_num:FindPos(666))
	print(t_list_num:Find(666))
	print(t_list_num:Front())
	print(t_list_num:Back())
	print(t_list_num:Len())
	t_list_num:Remove(2)
	t_list_num:Remove(19)
	t_list_num:RemovePos(1)
	print(t_list_num:PopFront())
	print(t_list_num:PopBack())
	--测试插入重复元素(应该成功)
	t_list_num:PushFront(666)
	print(t_list_num:Len())
	print(t_list_num:Empty())
	t_list_num:Clear()
	print(t_list_num:Empty())
	t_list_num:PushBack_TN(t0)
	print("t_list_num.Pos\t=",t_list_num.Pos)
	for i=1,t_list_num.Pos,1 do
		print(t_list_num.body[i])
	end
	t0 = t_list_num:PopFront_TN(3)
	print("t_list_num.Pos\t=",t_list_num.Pos)
	for i=1,t_list_num.Pos,1 do
		print(t_list_num.body[i])
	end
	print("t0:")
	for i=1,#t0,1 do
		print(t0[i])
	end
end

--启动自测
--local t_list_num=TabList:New("number"); if t_list_num ~= nil then Test(t_list_num) end

return TabList
