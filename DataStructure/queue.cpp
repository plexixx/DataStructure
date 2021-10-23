#pragma warning(disable:4996)
#pragma warning(disable:4101)

using namespace std;

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define CORRECT 1/*该字符串是回文串*/
#define ERROR  -1/*该字符串不是回文串*/ 
struct Node {
	char letter;/*输入的字符*/
	struct Node* Pre;/*前驱指针*/
	struct Node* Next;/*后驱指针*/
};
typedef struct Node* PtrToNode;
typedef struct Node* Position;
struct QNode {
	Position Front;/*队列的头指针*/
	Position Rear;/*队列的尾指针*/
};
typedef struct QNode* PtrToQNode;
typedef PtrToQNode Queue;
/*创建队列*/
Queue creatQueue()
{
	Queue Q = (Queue)malloc(sizeof(struct QNode));
	PtrToNode N = (PtrToNode)malloc(sizeof(struct Node));
	N->Next = NULL;
	N->Pre = N;
	Q->Front = N;
	Q->Rear = N;
	return Q;
}
/*判断队列是不是为空*/
bool ifEmpty(Queue Q)
{
	return (Q->Front->Next == NULL);
}
/*删除队列*/
void delQueue(Queue Q)
{
	PtrToNode Last;
	while (!ifEmpty(Q))
	{
		Last = Q->Front->Next;
		Q->Front = Last->Next;
		free(Last);
	}
	free(Q);
}
/*插入一个队列成员*/
void InsertQ(Queue Q, char X)
{/*newNode为新插入的结点*/
	PtrToNode newNode = (PtrToNode)malloc(sizeof(struct Node));
	newNode->letter = X;
	newNode->Next = NULL;
	newNode->Pre = Q->Rear;
	/*修改队列头尾指针*/
	Q->Rear->Next = newNode;
	Q->Rear = newNode;
	Q->Front->Pre = Q->Rear;
}
/*删除队列成员并返回删除的值*/
char queque_Pop(Queue Q)
{
	Position FrontCell;
	char FrontElem;/*FrontElem表示被删除的值*/

	if (ifEmpty(Q))
	{
		printf("队列空");
		return 0;
	}
	else {
		FrontCell = Q->Front->Next;
		if (Q->Front->Next->Next)
		{
			Q->Front->Next = Q->Front->Next->Next;
			Q->Front->Next->Pre = Q->Front;
		}
		else
		{
			Q->Front->Next = NULL;
			Q->Rear = Q->Front;
		}
		FrontElem = FrontCell->letter;
		free(FrontCell);/*释放被删除结点空间*/
		return FrontElem;
	}
}
/*弹出尾结点并返回删除的值*/
char stack_Pop(Queue Q)
{
	PtrToNode RearCell;
	char RearElem;

	RearCell = Q->Rear;
	RearElem = RearCell->letter;
	Q->Rear->Pre->Next = NULL;
	Q->Rear = RearCell->Pre;
	Q->Front->Pre = Q->Rear;
	free(RearCell);
	return RearElem;
}
/*工作函数*/
void work()
{
	Queue Q;
	char front, rear;/*分别用来表示队列头尾的字符*/
	char X;
	int cnt = 0;/*计数*/
	/*创建一个队列*/
	Q = creatQueue();
	do {/*将输入的字符全部插到队列中*/
		scanf("%c", &X);
		if (X != '#')
		{
			InsertQ(Q, X);
			cnt++;
		}
	} while (X != '#');
	if (cnt == 0)
		printf("非法的输入\n");
	int flag = 1;
	if (cnt)
	{
		while (!ifEmpty(Q))
		{/*不断弹出队列头和尾比较是否相同*/
			front = Q->Front->Next->letter;
			rear = Q->Rear->letter;
			if (front != rear)
			{
				flag = 0;
				break;
			}
			queque_Pop(Q);
			stack_Pop(Q);
		}
		if (flag == 1)
			printf("%d", CORRECT);
		else
			printf("%d", ERROR);
		/*删除队列*/
		delQueue(Q);
	}
}
int main(void)
{
	work();

	return 0;
}