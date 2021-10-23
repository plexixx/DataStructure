#include<stdio.h>
#include<stdlib.h> 
struct Node {
	int Value;//对应每个结点的序号
	struct Node* Pre;//结点前驱
	struct Node* Next;//结点后驱 
};
typedef struct Node* PtrToNode;
//创建循环链表（存在空头结点）,最后一个结点指向头结点 
PtrToNode creatList(int n)
{
	int i;
	PtrToNode head = (PtrToNode)malloc(sizeof(struct Node));
	PtrToNode p, q;//q表示创建循环链表的上一个结点，p表示新插入的结点 
	/*设置空头结点*/
	head->Next = head;
	head->Pre = head;
	head->Value = 0;
	q = head;
	/*插入n个结点*/
	for (i = 1; i <= n; i++)
	{
		p = (PtrToNode)malloc(sizeof(struct Node));
		p->Value = i;
		q->Next = p;
		p->Pre = q;
		p->Next = head;
		head->Pre = p;
		q = p;
	}


	return head;
}
//根据序号找到指向对应结点的指针 
PtrToNode findNode(PtrToNode head, int N)
{
	PtrToNode p, last;
	p = head;
	last = head->Pre;
	int i;
	/*判断给定的N是否合法*/
	if (N > last->Value || N < 1)
	{
		printf("Wrong N.\n");
		p = NULL;
	}
	/*找到第N个结点*/
	for (i = 0; i < N; i++)
	{
		p = p->Next;
	}
	return p;
}
//判断链表是否为空，是空则返回1
int ifEmpty(PtrToNode head)
{
	return (head->Next == NULL);
}
//删除指定结点
void delNode(PtrToNode head, PtrToNode now)
{
	PtrToNode nextNode, lastNode;
	if (now != NULL)
	{
		nextNode = now->Next;
		lastNode = now->Pre;
		lastNode->Next = nextNode;
		nextNode->Pre = lastNode;
		free(now);
	}
}
//工作函数，完成计算 
void work()
{
	int n, x, y;
	PtrToNode Circle, denode;
	printf("请输入队员数n,开始计数的战士编号x,以及数到让战士执行任务的数y：\n");
	scanf("%d,%d,%d", &n, &x, &y);
	Circle = creatList(n);
	/*循环删除结点*/
	while (!ifEmpty(Circle))
	{
		denode = findNode(Circle, (x + y) % n);
		/*当删除的是序号为1的结点时判断删除后链表是否为空*/
		if (denode->Value == 1) {
			delNode(Circle, denode);
			if (ifEmpty(Circle))
				printf("Correct n,x,y.\n");
			else
				printf("Incorrect n,x,y.\n");
			return;
		}
		else {
			delNode(Circle, denode);
		}
		/*每次删除后更新x和n的值*/
		x = (x + y) % n;
		n--;
	}
}
int main(void)
{
	work();

	return 0;
}
