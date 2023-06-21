1>:字符分类<wctype.h>
iswalnum()  (C95)  检查宽字符是否为字母或数字
iswalpha()  (C95)  检查宽字符是否为字母
iswlower()  (C95)  检查宽字符是否为小写
iswupper()  (C95)  检查宽字符是否为大写
iswdigit()  (C95)  检查宽字符是否为数字
iswxdigit() (C95)  检查宽字符是否为十六进制字符
iswcntrl()  (C95)  检查宽字符是否为控制字符
iswgraph()  (C95)  检查宽字符是否为图形字符
iswspace()  (C95)  检查宽字符是否为空白符
iswblank()  (C99)  检查宽字符是否为空格
iswprint()  (C95)  检查宽字符是否为打印字符
iswpunct()  (C95)  检查宽字符是否为标点符号
iswctype()  (C95)  按照指定的 LC_CTYPE 类别分类宽字符
wctype()    (C95)  查找当前 C 本地环境中的字符分类类别


2>:字符操作<wctype.h>
towlower()  (C95)  将宽字符转换为小写
towupper()  (C95)  将宽字符转换为大写
towctrans() (C95)  按照指定的 LC_TYPE 映射分类进行字符映射
wctrans()   (C95)  查找当前 C 本地环境中的字符映射类别


3>:转换成数值格式<wchar.h>
wcstol()  (C95)    long
wcstoll() (C99)    将宽字符串转换成整数值-long long
wcstoul() (C95)    long
wcstoull()(C99)    将宽字符串转换成无符号整数值-long long
wcstof()  (C99)    将宽字符串转换成浮点值-float
wcstod()  (C95)    double
wcstold() (C99)    将宽字符串转换成浮点值-long double
wcstoimax() (C99)  将宽字符串转换成 intmax_t, 定义于头文件 <inttypes.h>
wcstoumax() (C99)  将宽字符串转换成 uintmax_t, 定义于头文件 <inttypes.h>


4>:字符串操作<wchar.h>
wcscpy()  (C95)    将一个宽字符串复制给另一个
wcsncpy() (C95)    将一定量的宽字符从一个字符串复制到另一个
wcscat()  (C95)    将一个宽字符串的副本后附于另一个
wcsncat() (C95)    将一定量宽字符串从一个宽字符串后附到另一个
wcsxfrm() (C95)    变换宽字符串, 使得 wcscmp 会产生与 wcscoll 相同的结果


5>:字符串检验<wchar.h>
wcslen()  (C95)    返回宽字符串的长度
wcscmp()  (C95)    比较两个宽字符串
wcsncmp() (C95)    比较来自两个宽字符串的一定量字符
wcscoll() (C95)    根据当前本地环境比较两个宽字符串
wcschr()  (C95)    查找宽字符在宽字符串中的首次出现
wcsrchr() (C95)    查找宽字符在宽字符串中的最后一次出现
wcsspn()  (C95)    返回仅由另一个宽字符串中出现的宽字符分隔的最长首段长度
wcscspn() (C95)    返回仅由不出现于另一个宽字符串中的宽字符分隔的最长首段长度
wcspbrk() (C95)    查找一个宽字符串中的任何字符在另一个宽字符串中的首个位置
wcsstr()  (C95)    查找一个宽字符串在另一个宽字符串中的首次出现
wcstok()  (C95)    查找宽字符串中的下一个记号


6>:宽字符数组操作<wchar.h>
wmemcpy()  (C95)   在两个不重叠的数组间复制一定数量的宽字符
wmemmove() (C95)   在两个可能重叠的数组间复制一定数量的宽字符
wmemcmp()  (C95)   比较两个数组中一定数量的宽字符
wmemchr()  (C95)   在宽字符数组中查找宽字符的首次出现
wmemset()  (C95)   将给定的宽字符复制到宽字符数组的所有位置


7>:变量类型<wctype.h>
wchar_t            可保有任何合法宽字符的整数类型(C++ 关键词) <wchar.h>
wint_t    (C95)    可保有任何合法宽字符, 并至少多出一个值的整数类型
wctrans_t (C95)    保有本地环境限定的字符映射的标量类型
wctype_t  (C95)    保有本地环境限定的字符分类的标量类型


8>:宏<wchar.h>
WEOF()       (C95) 用于指示错误的 wint_t 类型的非字符值
WCHAR_MIN()  (C95) wchar_t 的最小合法值
WCHAR_MAX()  (C95) wchar_t 的最大合法值



