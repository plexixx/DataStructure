#include<bits/stdc++.h>

#pragma warning(disable:4996)
#pragma warning(disable:4101)

using namespace std;

struct Node
{
	int value;
	struct Node* pre;
	struct Node* next;
};

typedef struct Node node;

node* creatlist()//创建链表（标志结尾结束型）（存在空头结点）（空头结点前驱指向尾结点）
{
	struct Node* head;
	head = (node*)malloc(sizeof(node));
	head->pre = head;
	head->next = NULL;
	return head;
}


void dellist(node* head)//删除指定链表
{
	node* now = head;
	while (now != NULL)
	{
		head = head->next;
		free(now);
		now = head;
	}
	return;
}

void printlist(node* head)//打印链表
{
	node* now = head->next;
	if (now == NULL)printf("the list is empty\n");
	else
	{
		printf("%d ", now->value);
		cout << now->pre << endl;
		cout << now->next << endl;
		now = now->next;
		while (now != NULL)
		{
			printf(" %d ", now->value);
			cout << now->pre << endl;
			cout << now->next << endl;
			now = now->next;
		}
		printf("\n");
	}
	return;
}

int ifempty(node* head)//判断链表是否为空，为空则返回1
{
	if (head->next == NULL)return 1;
	else return 0;
}

void updatetail(node* head)//更新头结点前驱中储存的尾结点数据（未特殊注明的函数均不会更新尾指针）
{
	node* now = head;
	for (; now->next; now = now->next)continue;
	head->pre = now;
	return;
}

node* seeknode(node* head, int n)//查找结点，若有，返回指向第一个找到的指针，没有则返回NULL
{
	node* now;
	now = head->next;
	while (now != NULL)
	{
		if (now->value == n)return now;
		else now = now->next;
	}
	return NULL;
}

int lenlist(node* head)//统计链表长度
{
	int n = 0;
	while (head->next != NULL)
	{
		n++;
		head = head->next;
	}
	return n;
}

node* findnode(node* head, int n)//根据序号找到指向对应结点的指针
{
	n--;
	node* now;
	now = head->next;
	while (n && now != NULL)
	{
		now = now->next;
		n--;
	}
	return now;
}

void delnode_1(node* head, node* now)//删除指定结点，并更新尾结点
{
	if (head->pre == now)
	{
		head->pre = now->pre;
		free(now);
		head->pre->next = NULL;
		return;
	}
	else if (now->next == NULL)
	{
		now->pre->next = NULL;
		free(now);
		return;
	}
	else
	{
		now->pre->next = now->next;
		now->next->pre = now->pre;
		free(now);
		return;
	}
}

node* addnode(node* now, int n)//在指定结点后面添加结点，数值为n，返回新的尾结点
{
	node* new1;
	new1 = (node*)malloc(sizeof(node));
	new1->next = now->next;
	new1->value = n;
	new1->pre = now;
	now->next = new1;
	if (new1->next)new1->next->pre = new1;
	return new1;
}

void work()
{

	int n, x, y;
	cin >> n >> x >> y;
	if (/*输入的数据不符合要求*/0)return;
	node* head = creatlist();
	for (int i = 1; i <= n; i--)
	{
		head->pre = addnode(head->pre, i);
	}
	printlist(head);
	int ans;
	node* now = findnode(head, x);
	while (1)
	{
		for (int i = 1; i <= y - 1; i++)
			now = now->next;
		node* del = now;
		now = now->next;
		if (now == NULL)now = head->next;
		delnode_1(head, del);
		if (now->value == 1)break;
	}
	delnode_1(head, now);
	if (ifempty(head))
	{
		cout << 1;
	}
	else
	{
		cout << 0;
	}
}

int main(void)
{
	work();
	return 0;
}