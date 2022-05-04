--[[
说明:
	TabQueue = 先进先出

	table[1] 是队列Front()
	table[self.Pos] 是队列Back()
	PushBack(), PopFront(), 是先进先出
	PushFront(), 是优先级max 插入
	PopBack(), 是丢弃最后一个元素(最近刚插入的元素)

	拒绝使用c 语言array_queue 的做法:
		这样就不能造出一个无限增长的TabQueue,
		局限掉业务思维, 是极其愚蠢的, 高级语言不要用低级语言的做法,
		再有从性能方面考虑, c 语言array_queue 的做法, 必然产生大量if, 这其实性能也不会太好.


	每次PopFront(), PushFront(), 都会触发lua table 的'自身重新排序机制':
		PushFront() = table.insert()
		PopFront() = table.remove()
	(这个功能不需要你自己造, 直接使用table.remove() 即可, 
	 lua table会自己'删除/调整'下标指定的元素, 并触发重新排列)

	重新排序时, 直接使用table.sort()


缺点:
	元素太多, 性能会下跌, 元素太多, 可以考虑用linklist 链表list(可以频繁删除or 插入元素)
--]]



local TabQueue = {}

TabQueue.__index = TabQueue

--创建队列(Type = number/string/anything)
--成功返回table, 失败返回nil
function TabQueue:New( Type )
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
function TabQueue:Front()
	local Pos = self.Pos
	local Body = self.body
	return ((Pos == 0)  and  nil  or  Body[1])
end

--查看队尾元素(不操作, 只查看)
--成功返回body元素, 失败返回nil
function TabQueue:Back()
	local Pos = self.Pos
	local Body = self.body
	return ((Pos == 0)  and  nil  or  Body[Pos])
end

--从Back队尾插入
--成功返回true, 失败返回false
function TabQueue:PushBack( val )
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
function TabQueue:PopBack()
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
function TabQueue:PushFront( val )
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
function TabQueue:PopFront()
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
function TabQueue:Empty()
	local Pos = self.Pos
	return ((Pos == 0)  and  true  or  false)
end

--返回栈当前元素的长度
function TabQueue:Len()
	local Pos = self.Pos
	return Pos
end

--清空(重置)
function TabQueue:Clear()
	self.body = {}
	self.Pos = 0
end

--重新排序, 打乱TabQueue 原来的顺序[先进先出TabQueue禁用](直接使用table.sort()进行排序)
function TabQueue:Sort()
	local Sort = table.sort
	local Body = self.body
	Sort(Body)
end

--自测函数(不对外公开)
local function Test(queue_num)
	local i = 0
	queue_num:PushFront(19)
	queue_num:PushBack(1)
	queue_num:PushFront(1900)
	queue_num:PushBack(222)
	print("queue_num.Pos\t=",queue_num.Pos)
	for i=1,queue_num.Pos,1 do
		print(queue_num.body[i])
	end
	queue_num:Sort()
	print("queue_num.Pos\t=",queue_num.Pos)
	for i=1,queue_num.Pos,1 do
		print(queue_num.body[i])
	end
	print(queue_num:Front())
	print(queue_num:Back())
	print(queue_num:Len())
	print(queue_num:PopFront())
	print(queue_num:PopBack())
	print(queue_num:Len())
	print(queue_num:Empty())
	queue_num:Clear()
	print(queue_num:Empty())
end

--启动自测
local queue_num=TabQueue:New("number"); if queue_num ~= nil then Test(queue_num) end

return TabQueue
