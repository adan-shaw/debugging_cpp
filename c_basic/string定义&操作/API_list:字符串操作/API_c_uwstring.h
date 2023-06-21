1>:多字节/宽字符转换<stdlib.h>
mblen()    返回下一个多字节字符的字节数
mbtowc()   将下一个多字节字符转换成宽字符
wctomb()   将宽字符转换成其多字节表示
mbstowcs() 将窄多字节字符串转换成宽字符串
wcstombs() 将宽字符串转换成窄多字节字符串--<wchar.h>
mbsinit()  检查 mbstate_t 对象是否表示初始迁移状态
btowc()    将单字节窄字符加宽成宽字符, 倘若可能
wctob()    将宽字符收窄成单字节窄字符, 倘若可能
mbrlen()   给定状态, 返回下一个多字节字符的字节数
mbrtowc()  给定状态, 将下一个多字节字符转换成宽字符
wcrtomb()  给定状态, 将宽字符转换成其多字节表示
mbsrtowcs()给定状态, 将窄多字节字符串转换成宽字符串
wcsrtombs()给定状态, 将宽字符串转换成窄多字节字符串--<uchar.h>


2>:变量类型
mbstate_t()迭代多字节字符串所需的转换信息--<uchar.h>


3>:宏
MB_LEN_MAX 多字节字符的最大字节数(宏常量)--<stdlib.h>
MB_CUR_MAX 当前C本地环境中的多字节字符最大字节数(宏变量)--<uchar.h>



