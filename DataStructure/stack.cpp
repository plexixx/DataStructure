#pragma warning(disable:4996)
#pragma warning(disable:4101)
#include<stdio.h>
#include<stdlib.h> 
#include<limits.h>
#define ERROR -1
/*设置栈的结点*/
struct SNode {
	int value;
	struct SNode* Pre;
	struct SNode* Next;
};
typedef struct SNode* PtrToSNode;
typedef PtrToSNode Stack;
/*创建栈（存在空头结点）头结点的前驱指向尾结点*/
Stack creatStack()
{
	Stack S = (Stack)malloc(sizeof(struct SNode));
	/*存在空头结点*/
	S->Pre = S;
	S->Next = NULL;
	return S;
}
/*判断栈是否为空,为空则返回1*/
int ifEmpty(Stack S)
{
	return (S->Next == NULL);
}
/*删除栈*/
void delStack(Stack S)
{
	Stack LastCell = S->Pre;

	while (S->Next != NULL)
	{/*修正删除结点后,栈的指针*/
		LastCell->Pre->Next = NULL;
		S->Pre = LastCell->Pre;
		free(LastCell);
		LastCell = S->Pre;
	}
	free(S);
}
/*入栈操作*/
void Push(Stack S, int num)
{
	Stack newCell = (Stack)malloc(sizeof(struct SNode));
	/*修正插入结点之后的指针*/
	S->Pre->Next = newCell;
	newCell->Pre = S->Pre;
	newCell->value = num;
	newCell->Next = NULL;
	S->Pre = newCell;
}
/*出栈操作*/
int Pop(Stack S)
{/*thisCell表示栈顶结点,lastCell表示目前栈顶的前一个结点*/
	Stack thisCell, lastCell;
	int num;//用来表示栈顶弹出的数 

	thisCell = S->Pre;
	lastCell = thisCell->Pre;
	/*修正弹出栈顶后的结点指针*/
	lastCell->Next = NULL;
	S->Pre = lastCell;
	num = thisCell->value;
	free(thisCell);
	return num;
}
/*工作函数*/
void work()
{
	Stack A, B;
	char num;
	int  octal = 0, i;//八进制数 
	int  bin[3] = { 1,2,4 };//八进制数的权值  
	int  cnt1 = 0, flag = 1, cnt2 = 0;//计数和标志，用来查看输入是否为合法的 


	A = creatStack();
	B = creatStack();
	/*将二进制数压入A栈*/
	printf("请输入待转换的二进制数,以'#'结尾。\n");
	do {/*cnt1用来表示输入的二进制串的长度，cnt2用来表示输入的串的长度*/
		scanf("%c", &num);
		cnt2++;
		if (num == '0' || num == '1')
		{
			Push(A, num - '0');
			cnt1++;
		}
		else if (num != '#' || (num == '#' && cnt2 == 1))
		{/*如果输入为除二进制数和#以外的数，则为非法输入*/
			printf("非法输入\n");
			flag = ERROR;
			break;
		}
	} while (num != '#');

	if (cnt1 == 1 && Pop(A) == 0)/*当输入0时为特殊情况单独处理*/
		printf("0");
	else if (flag == 1)
	{
		while (!ifEmpty(A))
		{/*当栈A不为空的时候不断完成取出三个数转换成八进制数的过程*/
			for (i = 0; i < 3 && (!ifEmpty(A)); i++) {
				if (!ifEmpty(A))
				{/*每次出栈之前判断A栈是否为空*/
					octal += bin[i] * Pop(A);
					if (i == 2)//每次弹出三个数的时候入栈
					{
						Push(B, octal);
						octal = 0;//octal重新计数 
					}
				}
				if (ifEmpty(A) && octal)
					Push(B, octal);//满栈后将剩余的转化后的数压入B栈 
			}
		}
		while (!ifEmpty(B))//将B栈中的数依次弹出 
		{
			printf("%d", Pop(B));
		}
	}
	/*删除A和B两个栈*/
	delStack(A);
	delStack(B);
}
int main()
{
	printf("%d\n%d\n", INT_MAX, INT_MIN);
	work();
	return 0;
}
