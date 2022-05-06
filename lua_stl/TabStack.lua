--[[
说明:
	TabStack = 先进后出
--]]



local TabStack = {}

TabStack.__index = TabStack

--创建栈(Type = number/string/anything)
--成功返回table, 失败返回nil
function TabStack:New( Type )
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

--查看顶部元素(不操作, 只查看)
--成功返回body元素, 失败返回nil
function TabStack:Top()
	local Pos = self.Pos
	local Body = self.body
	return ((Pos <= 0)  and  nil  or  Body[Pos])
end

--入栈
--成功返回true, 失败返回false
function TabStack:Push( val )
	local Pos = self.Pos
	local Body = self.body
	--debug 时开启, 关掉性能更好
	--[[
	local Type = self.Type
	local val_type = type(val)
	if val_type ~= Type and Type ~= "anything" then
		return false
	end
	]]
	Pos = Pos + 1
	--更新Pos
	self.Pos = Pos
	Body[Pos] = val
	--return true
end

--出栈(pop 出时, 为了防止访问越界, 这里不能省if)
--成功返回body元素, 失败返回nil
function TabStack:Pop()
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

--Pop出整个Stack
--成功返回一个装满所有元素的table, 失败返回nil
function TabStack:PopAll()
	local Body = self.body
	self.Pos = 0
	self.body = {}
	return Body
end

--栈空?
--成功返回true, 失败返回false
function TabStack:Empty()
	local Pos = self.Pos
	return ((Pos <= 0)  and  true  or  false)
end

--返回栈当前元素的长度
function TabStack:Len()
	local Pos = self.Pos
	return Pos
end

--清空(重置)
function TabStack:Clear()
	self.body = {}
	self.Pos = 0
end

--自测函数(不对外公开)
local function Test(t_stack_num)
	local t = nil
	local i = 1
	t_stack_num:Push(19)
	t_stack_num:Push(1)
	t_stack_num:Push(1900)
	print(t_stack_num:Top())
	print(t_stack_num:Len())
	print(t_stack_num:Pop())
	print(t_stack_num:Len())
	print(t_stack_num:Empty())
	t_stack_num:Clear()
	print(t_stack_num:Empty())
	t_stack_num:Push(19)
	t_stack_num:Push(1)
	t_stack_num:Push(1900)
	print(t_stack_num:Top())
	print("PopAll:")
	t = t_stack_num:PopAll()
	for i=1,#t,1 do
		print(t[i])
	end
end

--启动自测
local t_stack_num=TabStack:New("number"); if t_stack_num ~= nil then Test(t_stack_num) end

return TabStack
