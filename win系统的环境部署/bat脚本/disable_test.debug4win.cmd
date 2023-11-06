:: ps: 
::   windows bat/cmd 脚本, 要用windows 的换行符号;
::   编码可以是utf-8, 这个问题不大;
::   否则备注会失效, 换行会失效, 执行会报错;



:: 本脚本需要先在bios 中关闭安全启动才能运行, 运行成功之后, 可以重新打开boot 中的安全启动

echo on
::
:: 通过自启动的方式, 每次开机禁用这些win 核心选项
::
:: 启动时, 关闭Windows测试模式



:: 禁用驱动签名
bcdedit -set loadoptions DDISABLE_INTEGRITY_CHECKS

:: 启用驱动签名
bcdedit -set loadoptions ENABLE_INTEGRITY_CHECKS


:: 安全模式启动
bcdedit -set nointegritychecks off


:: 正常模式启动
bcdedit -set TESTSIGNING off


:: 启动时, 关闭Windows 操作系统的内核调试模式
bcdedit -set DEBUG off


:: 启动时, 关闭Windows 操作系统boot启动调试模式
bcdedit -set BOOTDEBUG off




