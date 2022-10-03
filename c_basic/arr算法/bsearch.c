//编译:
//		gcc bsearch.c -g3 -o x 



//二分查找
/*
	说明:
		元素必须是有序的, 如果是无序的则要先进行排序操作.

	基本思想:
		二分查找也称为是折半查找, 属于有序查找算法.
		用给定值k先与中间结点的关键字比较, 中间结点把线形表分成两个子表,
		若相等则查找成功;
		若不相等, 再根据k与该中间结点关键字的比较结果确定下一步查找哪个子表,
		这样递归进行, 直到查找到或查找结束发现表中没有这样的结点.

	复杂度分析:
		最坏情况下, 关键词比较次数为log2(n+1), 且期望时间复杂度为O(log2n);

	注:
		折半查找的前提条件是需要有序表顺序存储,
		对于静态查找表, 一次排序后不再变化, 折半查找能得到不错的效率.
		但对于需要频繁执行插入或删除操作的数据集来说,
		维护有序的排序会带来不小的工作量, 那就不建议使用.

	ps:
		二分法的前提是, 必须是'有序'队列. 可以是数组, 链表, 二叉树.
*/



#include <stdio.h>



//#define arr_t int		//兼容int整形
#define arr_t double//兼容double浮点



//二分法(找到返回下标pos, 找不到返回-1)
int arr_bsearch(arr_t *arr, const int arr_len, arr_t target){
	int low=0, high=arr_len-1, pos;

	while(low <= high){
		pos = (low + high) / 2;
		if(target < arr[pos])
			high = pos - 1;
		else if(target > arr[pos])
			low = pos + 1;
		else
			/*
				可能存在所个相同值, 但随机返回其中一个值的pos,
				[pos+1], [pos-1]都有可能是相同值, 
				但为防止point越界访问, 直接找到pos, 就返回pos, 不探索[pos+1], [pos-1]
			*/
			return pos;
	}

	return -1;
}

//根据arr数组的pos, 查找该pos 前后有多少个相同的值(永远不会失败的函数)
int arr_get_same_count(arr_t *arr, const int arr_len, const int pos){
	int tmp=pos, count=0;
	while(tmp > 0)
		if(arr[--tmp] != arr[pos])
			break;
	count = pos - tmp;
	tmp = pos;
	while(tmp < arr_len)
		if(arr[tmp++] != arr[pos])
			break;
	return count + ((tmp-1) - pos);
}

int main(void){
	//							 0	1	 2	 3	 4	 5	 6	 7	 8	 9
	arr_t array[] = {0, 5, 12, 32, 41, 53, 64, 64, 64, 64};
	arr_t target = 64;
	int pos, count = 0;

	pos = arr_bsearch(array, 10, target);
	//printf("target=%d pos=%d\n", target, pos);
	printf("target=%lf pos=%d\n", target, pos);

	count = arr_get_same_count(array, 10, pos);
	printf("pos=%d, count=%d\n", pos, count);

	return 0;
}
