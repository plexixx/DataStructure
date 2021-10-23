#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include<fstream>
#include<string.h>

using namespace std;

typedef struct eList
{
	int weight;
	int anothernode;
	struct eList* next;
}elist;

void printmatrix(vector<vector<int>>matrixnow)//打印矩阵
{
	for (int i = 0; i < matrixnow.size(); i++)
	{
		for (int j = 0; j < matrixnow[i].size(); j++)
		{
			if (matrixnow[i][j] >= 0x1fffffff)
				cout << "*" << "	";
			else
				cout << matrixnow[i][j] << "	";
		}

		cout << endl;
	}
	cout << endl;
	return;
}


vector<elist*>readnodelist()//以三元组表形式读入邻接表（可调整）
{
	int n, vnumber;
	cout << "请输入节点数" << endl;
	cin >> vnumber;
	cout << "请输入三元组表数" << endl;
	cin >> n;
	cout << "请输入三元组表（注意：图不可以包含回路！）" << endl;
	vector<elist*>nodelist(vnumber);
	for (int i = 0; i < nodelist.size(); i++)nodelist[i] = NULL;
	for (int i = 0; i < n; i++)
	{
		int u, v, weight;
		cin >> u >> v >> weight;
		elist* newedge = (elist*)malloc(sizeof(elist));
		if (!newedge) { cout << "malloc_error" << endl; break; }
		u--; v--;
		newedge->anothernode = v;
		newedge->weight = weight;
		newedge->next = NULL;
		elist* now = nodelist[u];
		if (now)
		{
			while (now->next)now = now->next;
			now->next = newedge;
		}
		else
		{
			nodelist[u] = newedge;
		}
	}
	return nodelist;
}

vector<vector<int>>transformlisttomatrix(vector<elist*>nodelist)//将邻接表转换为邻接矩阵
{
	vector<vector<int>>edgematrix;
	for (int i = 0; i < nodelist.size(); i++)
	{
		edgematrix.push_back(vector<int>(nodelist.size()));
	}
	for (int i = 0; i < nodelist.size(); i++)
		for (int j = 0; j < nodelist.size(); j++)
			edgematrix[i][j] = 0x2fffffff;
	for (int i = 0; i < nodelist.size(); i++)
	{
		elist* now = nodelist[i];
		while (now)
		{
			edgematrix[i][now->anothernode] = 0 - now->weight;
			now = now->next;
		}
	}
	return edgematrix;
}

vector<vector<int>>findbiggestmatrix(vector<vector<int>>edgematrix)//使用Floyd算法计算任意两点间的最长（短）路
{
	for (int k = 0; k < edgematrix.size(); k++)
		for (int i = 0; i < edgematrix.size(); i++)
			for (int j = 0; j < edgematrix.size(); j++)
				if (edgematrix[i][j] > edgematrix[i][k] + edgematrix[k][j])
					edgematrix[i][j] = edgematrix[i][k] + edgematrix[k][j];
	return edgematrix;
}

void findandprintfarthestnode(vector<vector<int>>edgematrix)
{
	int maxi = 0;
	int maxj = 0;
	for (int i = 0; i < edgematrix.size(); i++)
		for (int j = 0; j < edgematrix.size(); j++)
			if (edgematrix[i][j] < edgematrix[maxi][maxj])
			{
				maxi = i;
				maxj = j;
			}
	cout << "距离最远的两点为：" << endl;
	cout << maxi + 1 << " " << maxj + 1 << endl;
	cout << "其距离为：" << endl;
	cout << 0 - edgematrix[maxi][maxj] << endl;
	return;
}

void work()//总工作函数
{
	vector<elist*>nodelist = readnodelist();
	cout << "正在计算……" << endl;
	vector<vector<int>>edgematrix = transformlisttomatrix(nodelist);
	//printmatrix(edgematrix);
	vector<vector<int>>resultmatrix = findbiggestmatrix(edgematrix);
	//printmatrix(resultmatrix);
	findandprintfarthestnode(resultmatrix);
	return;
}

int main()//总调度函数
{
	work();
	return 0;
}

/*
5
4
1 2 1
2 3 1
3 4 1
4 5 1

*/