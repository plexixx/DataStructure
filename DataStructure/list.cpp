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

node* creatlist()//����������־��β�����ͣ������ڿ�ͷ��㣩����ͷ���ǰ��ָ��β��㣩
{
	struct Node* head;
	head = (node*)malloc(sizeof(node));
	head->pre = head;
	head->next = NULL;
	return head;
}


void dellist(node* head)//ɾ��ָ������
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

void printlist(node* head)//��ӡ����
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

int ifempty(node* head)//�ж������Ƿ�Ϊ�գ�Ϊ���򷵻�1
{
	if (head->next == NULL)return 1;
	else return 0;
}

void updatetail(node* head)//����ͷ���ǰ���д����β������ݣ�δ����ע���ĺ������������βָ�룩
{
	node* now = head;
	for (; now->next; now = now->next)continue;
	head->pre = now;
	return;
}

node* seeknode(node* head, int n)//���ҽ�㣬���У�����ָ���һ���ҵ���ָ�룬û���򷵻�NULL
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

int lenlist(node* head)//ͳ��������
{
	int n = 0;
	while (head->next != NULL)
	{
		n++;
		head = head->next;
	}
	return n;
}

node* findnode(node* head, int n)//��������ҵ�ָ���Ӧ����ָ��
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

void delnode_1(node* head, node* now)//ɾ��ָ����㣬������β���
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

node* addnode(node* now, int n)//��ָ����������ӽ�㣬��ֵΪn�������µ�β���
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
	if (/*��������ݲ�����Ҫ��*/0)return;
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