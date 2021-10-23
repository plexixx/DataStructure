#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include<fstream>
#include<string.h>

#define MaxSize 1000
#define MAXCOUNT 100
#define TREEARRAYCOUNT 100

using namespace std;

typedef struct BiTreeNode           //二叉树
{
	char letter;
	int ifl;                       //为1表示左孩子为空，为0表示左满右空，为2表示左右皆满
	int level;                     //用于指示结点在哪一层
	BiTreeNode* lchild, * rchild;
	BiTreeNode* parents;
}BiTree, * PtrToBiTree;

int nodeNum = 0;//记录二叉树有效结点数

PtrToBiTree CreatBiTree()//创建树
{
	PtrToBiTree root = (PtrToBiTree)malloc(sizeof(BiTreeNode));
	PtrToBiTree nodenow = root;
	char now;
	cin >> now;
	nodenow->letter = now;
	nodenow->ifl = 1;
	cin >> now;
	while (now)//判断输入是否结束
	{
		//cout << nodenow->letter << endl;
		while (nodenow->ifl == 2)//如果左右皆满，则回溯
		{
			if (nodenow == root)break;//回到根则跳出
			nodenow = nodenow->parents;
		}
		if (now == '0')break;
		//if ( nodenow==root && nodenow->ifl = 2)break;
		else if (now != '*')//不为*，说明为有效值
		{
			if (nodenow->ifl == 1)//左孩子为空，则存至左孩子，置ifl为0
			{
				nodenow->lchild = (PtrToBiTree)malloc(sizeof(BiTreeNode));
				nodenow->lchild->parents = nodenow;
				nodenow->ifl = 0;
				nodenow->lchild->letter = now;
				nodenow = nodenow->lchild;
				nodenow->ifl = 1;//新结点ifl置为1
			}
			else//左孩子不为空，则存至右孩子，置ifl为2
			{
				nodenow->rchild = (PtrToBiTree)malloc(sizeof(BiTreeNode));
				nodenow->rchild->parents = nodenow;
				nodenow->ifl = 2;
				nodenow->rchild->letter = now;
				nodenow = nodenow->rchild;
				nodenow->ifl = 1;//新结点ifl置为1
			}
		}
		else//为*，则为无效值
		{
			if (nodenow->ifl == 1)//左孩子为空，则让左孩子为NULL，ifl置为0
			{
				nodenow->lchild = (PtrToBiTree)malloc(sizeof(BiTreeNode));
				nodenow->lchild->parents = nodenow;
				nodenow->lchild->letter = '*';
				nodenow->lchild->lchild = NULL;
				nodenow->lchild->rchild = NULL;
				nodenow->ifl = 0;
			}
			else//左孩子不为空，则让右孩子为NULL，ifl置为2
			{
				nodenow->rchild = (PtrToBiTree)malloc(sizeof(BiTreeNode));
				nodenow->rchild->parents = nodenow;
				nodenow->rchild->letter = '*';
				//if(nodenow->lchild==)
				nodenow->rchild->lchild = NULL;
				nodenow->rchild->rchild = NULL;
				nodenow->ifl = 2;
			}
			
		}
		cin >> now;
		nodeNum++;
	}
	return root;
}
#define MaxSize 1000
#define MAXCOUNT 100
#define TREEARRAYCOUNT 100
const int BPOW[] = { 0, 1, 3, 7, 15, 31, 63, 127,255,511,1023,2047 };//打印空白单位的个数

typedef struct queue {
	BiTree* numQ[MaxSize];
	int front;
	int rear;
}Queue;

Queue Q;

void initilize() { //初始化队列
	Q.front = 0;
	Q.rear = 0;
}

void Push(BiTree* root) { //入队
	Q.numQ[++Q.rear] = root;
}

BiTree* Pop() { //出队
	return Q.numQ[++Q.front];
}

int ifempty() { //判断对列是否为空
	return Q.rear == Q.front;
}

char* LevelOrderTraversal(BiTree* root, int length,int h) { //二叉树的层次遍历
	BiTree* temp;
	char* tree;
	tree = (char*)malloc(length * sizeof(char));
	int i = 0;

	Push(root);
	while (!ifempty()) {
		temp = Pop();
		tree[i] = temp->letter;//将队首结点存储
		i++;
		if (temp->lchild)     //把Pop掉的结点的左子结点加入队列
			Push(temp->lchild);
		if (temp->rchild)  //  把Pop掉的结点的右子结点加入队列
			Push(temp->rchild);
	}
	return tree;
}
void printBlank(int n) {//打印n个空白单位
	for (int i = 0; i < n; i++)
		printf(" ");//按需调整“    ”
}

void printBinTree(char oldTree[], int l) {//传入一个按照层序存储的数组oldTree
	int nodeCount = l;//最大行空格 数据共有的大小
	int MaxL = -1;//用来记录最大行数
	char* tree = oldTree;//从oldTree的有效数开始存入数组tree中
	char treeArray[TREEARRAYCOUNT][TREEARRAYCOUNT] = {};
	for (int i = 0; i < nodeCount;) {//分层,建立一个二维数组用来存储二叉树每一层的字符，最大为2^i-1
		MaxL++;
		for (int j = 0; j <= BPOW[MaxL] && i < nodeCount;)
			treeArray[MaxL][j++] = tree[i++];
	}
	MaxL++;//Better view
	for (int k = 0; k <= MaxL; k++) {//打印
		printBlank(BPOW[MaxL - k]);
		//BPOW[k]+i<nodeCount，一个不多，一个不少
		for (int i = 0; i <= BPOW[k] && (BPOW[k] + i < nodeCount); i++) {
			if (treeArray[k][i] == '*')
				printf(" ");
			else
				printf("%c", treeArray[k][i]);//按需调整“    ”
			printBlank(BPOW[MaxL - k + 1]);
		}
		printf("\n");
	}
	printf("-----------------------------------\n");
}

int GetHeight(BiTree* BT,int i)
{
	int hl, hr, max;
	if (BT != NULL)
	{
		BT->level = i;
		hl = GetHeight(BT->lchild, i + 1);
		hr = GetHeight(BT->rchild, i + 1);
		max = hl > hr ? hl : hr;
		return (max + 1);
	}
	else return 0;
}

void EdifyTree(PtrToBiTree t, int h)
{
	/*
	if (t->level < h)
	{
		if (t->letter == '*')
		{
			t->lchild = (PtrToBiTree)malloc(sizeof(BiTree));
			t->rchild = (PtrToBiTree)malloc(sizeof(BiTree));
			t->lchild->letter = '*';
			t->rchild->letter = '*';
			t->lchild->level = t->level + 1;
			t->rchild->level = t->level + 1;
		}
	}
	EdifyTree(t->lchild, h);
	EdifyTree(t->rchild, h);
	*/
	PtrToBiTree temp;
	Push(t);
	
	while (!ifempty())
	{
		temp = Pop();
		/*如果某个结点的左子树为空或者右子树为空,那么就新建一个*结点并入栈*/
		/*特殊情况是弹出来是'*'结点,而这时需要新建左子树和右子树,于是需要同时进入两个分支(所以用两个if)*/
		/*如果到最后一层不进行任何新建结点的操作,所以用tall和height标记*/
		if (temp->level < h)
		{
			if (temp->lchild == NULL && temp->letter == '*')
			{//如果左子树为空或者遇到*,则新建一个*结点
				PtrToBiTree newtree1 = (PtrToBiTree)malloc(sizeof(BiTree));
				temp->lchild = newtree1;
				newtree1->level = temp->level + 1;
				newtree1->letter = '*';
				newtree1->lchild = NULL;
				newtree1->rchild = NULL;
				newtree1->parents = temp;
			}
			if (temp->rchild == NULL && temp->letter == '*')
			{//如果右子树为空或者遇到*,则新建一个*结点
				PtrToBiTree newtree2 = (BiTree*)malloc(sizeof(BiTree));
				temp->rchild = newtree2;
				newtree2->level = temp->level + 1;
				newtree2->letter = '*';
				newtree2->lchild = NULL;
				newtree2->rchild = NULL;
				temp->rchild = newtree2;
				newtree2->parents = temp;
			}
		}
		if (temp->lchild&&temp->lchild->level<h)Push(temp->lchild);
		if (temp->rchild&&temp->rchild->level<h)Push(temp->rchild);
	}
}
void work()
{
	PtrToBiTree tree = CreatBiTree();
	int maxHeight = GetHeight(tree, 1);
	int length = nodeNum;
	EdifyTree(tree, maxHeight);
	char* treeList = LevelOrderTraversal(tree, length, maxHeight);//得到树的层序数组
	printBinTree(treeList, length);//打印树
	return;
}

int main()
{
	work();
	return 0;
}

//abdh**i***cfl**m**gn**o**0