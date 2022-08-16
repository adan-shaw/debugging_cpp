//只在arr_stack_mini 的基础上, 添加批量操作函数(需要开启__as_pushN 宏)
#include "arr_stack_mini.h"
#include <stdbool.h>



//压栈n 个(需要先将N 个元素放进pas->cache 缓冲区, 然后传入n 元素个数)
bool as_pushN(as_t* pas, unsigned int N);


//弹栈n 个(传入n 元素个数, 将N 个元素弹入pas->cache 缓冲区, 然后自己取走)
bool as_popN(as_t* pas, unsigned int N);
