--[[
说明:
	继承所有TabList 的特性, 剪裁出TabSet, 
	元素少的情况下, 也可以适当频繁插入/删除(最好就不要频繁插入/删除)

	TabSet 不允许有重复元素的出现
--]]



local tabList = require "TabList"

local TabSet = {}

TabSet.__index = TabSet

--创建队列(Type = number/string/anything)
--成功返回table, 失败返回nil
--[自己保证元素的一致性, 否则没办法排序]
function TabSet:New( Type )
	local Setmetatable = setmetatable
	local t = {}
	t.tList = tabList:New(Type)
	if t.tList == nil then
		return nil
	else
		Setmetatable(t, self)
		t.__index = self
		return t
	end
end

--查看队头元素(不操作, 只查看)
--成功返回body元素, 失败返回nil
function TabSet:Front()
	return self.tList:Front()
end

--查看队尾元素(不操作, 只查看)
--成功返回body元素, 失败返回nil
function TabSet:Back()
	return self.tList:Back()
end

--从Back队尾插入
--成功返回true, 失败返回false(debug only)
function TabSet:PushBack( val )
	--TabSet 不允许重复元素
	local tmp = self.tList:Find(val)
	if tmp == nil then
		--debug 选项
		--return self.tList:PushBack(val)
		self.tList:PushBack(val)
	end
end

--从Back队尾弹出
--成功返回body元素, 失败返回nil
function TabSet:PopBack()
	return self.tList:PopBack()
end

--从Front队头插入
--成功返回true, 失败返回false(debug only)
function TabSet:PushFront( val )
	--TabSet 不允许重复元素
	local tmp = self.tList:Find(val)
	if tmp == nil then
		--debug 选项
		--return self.tList:PushFront(val)
		self.tList:PushFront(val)
	end
end

--从Front队头弹出
--成功返回body元素, 失败返回nil
function TabSet:PopFront()
	return self.tList:PopFront()
end

--从Back队尾插入装有n 个元素的连续table(必须保证元素类型一致性, 遵守number/string/anything 原则)
--自己保证t 不能有重复元素
function TabSet:PushBack_TN( t )
	self.tList:PushBack_TN(t)
end

--从Front队头弹出装有n 个元素的连续table
--成功返回table, 失败返回nil
function TabSet:PopFront_TN( n )
	return self.tList:PopFront_TN(n)
end

--栈空?
--成功返回true, 失败返回false
function TabSet:Empty()
	return self.tList:Empty()
end

--返回栈当前元素的长度
function TabSet:Len()
	return self.tList:Len()
end

--清空(重置)
function TabSet:Clear()
	self.tList:Clear()
end

--重新排序, 打乱TabSet 原来的顺序[先进先出TabSet禁用](直接使用table.sort()进行排序)
function TabSet:Sort()
	self.tList:Sort()
end

--洗牌乱序, 打乱TabSet 原来的顺序, 随机排序
function TabSet:Shuffle()
	self.tList:Shuffle()
end

--查找元素(不是pop, 只查找, 不删除)
--成功返回body元素, 失败返回nil
function TabSet:Find( val )
	return self.tList:Find(val)
end

--查找元素Pos
--成功返回body元素的Pos, 失败返回nil
function TabSet:FindPos( val )
	return self.tList:FindPos(val)
end

--根据pos 位置值, 插入元素(触发table重新排序)
function TabSet:InsertPos( Pos, val )
	--TabSet 不允许重复元素
	local tmp = self.tList:Find(val)
	if tmp == nil then
		self.tList:InsertPos(Pos, val)
	end
end

--根据pos 位置值, 删除元素(触发table重新排序)
--成功返回true, 失败返回false(debug only)
function TabSet:Remove( val )
	--debug 选项
	--return self.tList:Remove(val)
	self.tList:Remove(val)
end

--根据pos 位置值, 删除元素(触发table重新排序) [这个函数少用!! 不安全]
--成功返回true, 失败返回false(debug only)
function TabSet:RemovePos( pos )
	--debug 选项
	--return self.tList:RemovePos(pos)
	self.tList:RemovePos(pos)
end

--自测函数(不对外公开)
local function Test(t_set_num)
	local i = 0
	local t0 = { 99, 88, 6, 7, 33, 1 }
	t_set_num:PushFront(19)
	t_set_num:PushBack(1)
	t_set_num:PushFront(1900)
	t_set_num:PushBack(222)
	t_set_num:InsertPos(2, 666)
	print("t_set_num.tList.Pos\t=",t_set_num.tList.Pos)
	for i=1,t_set_num.tList.Pos,1 do
		print(t_set_num.tList.body[i])
	end
	t_set_num:Shuffle()
	print("t_set_num.tList.Pos\t=",t_set_num.tList.Pos)
	for i=1,t_set_num.tList.Pos,1 do
		print(t_set_num.tList.body[i])
	end
	t_set_num:Sort()
	print("t_set_num.tList.Pos\t=",t_set_num.tList.Pos)
	for i=1,t_set_num.tList.Pos,1 do
		print(t_set_num.tList.body[i])
	end
	print(t_set_num:FindPos(666))
	print(t_set_num:Find(666))
	print(t_set_num:Front())
	print(t_set_num:Back())
	print(t_set_num:Len())
	t_set_num:Remove(2)
	t_set_num:Remove(19)
	t_set_num:RemovePos(1)
	print(t_set_num:PopFront())
	print(t_set_num:PopBack())
	print("t_set_num.tList.Pos\t=",t_set_num.tList.Pos)
	for i=1,t_set_num.tList.Pos,1 do
		print(t_set_num.tList.body[i])
	end
	--测试插入重复元素(应该失败)
	t_set_num:PushFront(666)
	print(t_set_num:Len())
	print(t_set_num:Empty())
	t_set_num:Clear()
	print(t_set_num:Empty())
	t_set_num:PushBack_TN(t0)
	print("t_set_num.tList.Pos\t=",t_set_num.tList.Pos)
	for i=1,t_set_num.tList.Pos,1 do
		print(t_set_num.tList.body[i])
	end
	t0 = t_set_num:PopFront_TN(3)
	print("t_set_num.tList.Pos\t=",t_set_num.tList.Pos)
	for i=1,t_set_num.tList.Pos,1 do
		print(t_set_num.tList.body[i])
	end
	print("t0:")
	for i=1,#t0,1 do
		print(t0[i])
	end
end

--启动自测
local t_set_num=TabSet:New("number"); if t_set_num ~= nil then Test(t_set_num) end

return TabSet
