#include<stdio.h>
#include<stdlib.h> 
struct Node {
	int Value;//��Ӧÿ���������
	struct Node* Pre;//���ǰ��
	struct Node* Next;//������ 
};
typedef struct Node* PtrToNode;
//����ѭ���������ڿ�ͷ��㣩,���һ�����ָ��ͷ��� 
PtrToNode creatList(int n)
{
	int i;
	PtrToNode head = (PtrToNode)malloc(sizeof(struct Node));
	PtrToNode p, q;//q��ʾ����ѭ���������һ����㣬p��ʾ�²���Ľ�� 
	/*���ÿ�ͷ���*/
	head->Next = head;
	head->Pre = head;
	head->Value = 0;
	q = head;
	/*����n�����*/
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
//��������ҵ�ָ���Ӧ����ָ�� 
PtrToNode findNode(PtrToNode head, int N)
{
	PtrToNode p, last;
	p = head;
	last = head->Pre;
	int i;
	/*�жϸ�����N�Ƿ�Ϸ�*/
	if (N > last->Value || N < 1)
	{
		printf("Wrong N.\n");
		p = NULL;
	}
	/*�ҵ���N�����*/
	for (i = 0; i < N; i++)
	{
		p = p->Next;
	}
	return p;
}
//�ж������Ƿ�Ϊ�գ��ǿ��򷵻�1
int ifEmpty(PtrToNode head)
{
	return (head->Next == NULL);
}
//ɾ��ָ�����
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
//������������ɼ��� 
void work()
{
	int n, x, y;
	PtrToNode Circle, denode;
	printf("�������Ա��n,��ʼ������սʿ���x,�Լ�������սʿִ���������y��\n");
	scanf("%d,%d,%d", &n, &x, &y);
	Circle = creatList(n);
	/*ѭ��ɾ�����*/
	while (!ifEmpty(Circle))
	{
		denode = findNode(Circle, (x + y) % n);
		/*��ɾ���������Ϊ1�Ľ��ʱ�ж�ɾ���������Ƿ�Ϊ��*/
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
		/*ÿ��ɾ�������x��n��ֵ*/
		x = (x + y) % n;
		n--;
	}
}
int main(void)
{
	work();

	return 0;
}
