--[[
说明:
	在TabQueue 的基础上, 改造出TabSet, 元素少的情况下, 也可以适当频繁插入/删除
	(最好就不要频繁插入/删除)
--]]



local TabSet = {}

TabSet.__index = TabSet

--创建队列(Type = number/string/anything)
--成功返回table, 失败返回nil
function TabSet:New( Type )
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
function TabSet:Front()
	local Pos = self.Pos
	local Body = self.body
	return ((Pos == 0)  and  nil  or  Body[1])
end

--查看队尾元素(不操作, 只查看)
--成功返回body元素, 失败返回nil
function TabSet:Back()
	local Pos = self.Pos
	local Body = self.body
	return ((Pos == 0)  and  nil  or  Body[Pos])
end

--从Back队尾插入
--成功返回true, 失败返回false
function TabSet:PushBack( val )
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
	return true
end

--从Back队尾弹出
--成功返回body元素, 失败返回nil
function TabSet:PopBack()
	local Pos = self.Pos
	local Body = self.body
	if Pos == 0 then
		return nil
	else
		Pos = Pos - 1
		--更新Pos
		self.Pos = Pos
		return Body[Pos + 1]
	end
end

--从Front队头插入
--成功返回true, 失败返回false
function TabSet:PushFront( val )
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
	return true
end

--从Front队头弹出
--成功返回body元素, 失败返回nil
function TabSet:PopFront()
	local Pos = self.Pos
	local ret = nil
	local Remove = table.remove
	local Body = self.body
	if Pos == 0 then
		return nil
	else
		ret = Body[1]
		Remove(Body, 1)
		--更新Pos
		self.Pos = Pos - 1
		return ret
	end
end

--栈空?
--成功返回true, 失败返回false
function TabSet:Empty()
	local Pos = self.Pos
	return ((Pos == 0)  and  true  or  false)
end

--返回栈当前元素的长度
function TabSet:Len()
	local Pos = self.Pos
	return Pos
end

--清空(重置)
function TabSet:Clear()
	self.body = {}
	self.Pos = 0
end

--重新排序, 打乱TabSet 原来的顺序[先进先出TabSet禁用](直接使用table.sort()进行排序)
function TabSet:Sort()
	local Sort = table.sort
	local Body = self.body
	Sort(Body)
end

--查找元素(不是pop, 只查找, 不删除)
--成功返回body元素, 失败返回nil
function TabSet:Find( val )
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
function TabSet:FindPos( val )
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

--任意pos 插入(触发table重新排序)
function TabSet:Insert( Pos, val )
	local Insert = table.insert
	local Body = self.body
	local Pos = self.Pos
	Insert(Body, Pos, val)
	self.Pos = Pos + 1
end

--任意pos 删除(触发table重新排序)
function TabSet:Remove( Pos )
	local Remove = table.remove
	local Body = self.body
	local Pos = self.Pos
	Remove(Body, Pos)
	self.Pos = Pos - 1
end

--自测函数(不对外公开)
local function Test(t_set_num)
	local i = 0
	t_set_num:PushFront(19)
	t_set_num:PushBack(1)
	t_set_num:PushFront(1900)
	t_set_num:PushBack(222)
	t_set_num:Insert(2, 666)
	print("t_set_num.Pos\t=",t_set_num.Pos)
	for i=1,t_set_num.Pos,1 do
		print(t_set_num.body[i])
	end
	t_set_num:Sort()
	print("t_set_num.Pos\t=",t_set_num.Pos)
	for i=1,t_set_num.Pos,1 do
		print(t_set_num.body[i])
	end
	print(t_set_num:FindPos(666))
	print(t_set_num:Find(666))
	print(t_set_num:Front())
	print(t_set_num:Back())
	print(t_set_num:Len())
	t_set_num:Remove(2)
	print(t_set_num:PopFront())
	print(t_set_num:PopBack())
	print(t_set_num:Len())
	print(t_set_num:Empty())
	t_set_num:Clear()
	print(t_set_num:Empty())
end

--启动自测
local t_set_num=TabSet:New("number"); if t_set_num ~= nil then Test(t_set_num) end

return TabSet
