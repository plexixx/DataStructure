#pragma warning(disable:4996)
#pragma warning(disable:4101)
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<iostream>

#define MAX 100
using namespace std;

/*三元组结构体*/
typedef struct
{
	int row, col;//行标、列标
	int value;//元素值
}triple;

typedef struct Mnode
{
	triple data[MAX];//存储所有非零元素的三元组
	int row_num, col_num, num;//row_num和col_num分别记录矩阵的行数和列数，num为矩阵中所有非零元素总数
}*Matrix;

/*创建三元组表*/
Matrix createMatrix(int row_n, int col_n, int n)
{
	Matrix M = (Matrix)malloc(sizeof(struct Mnode));
	int row, col, value;
	M->num = n;
	M->col_num = 0; M->row_num = 0;//初始化各参数
	//若矩阵总行数/总列数/非零元素个数小于0，判定为非法输入
	if (row_n < 0 || col_n < 0 || n < 0)
	{
		printf("非法输入\n");
		abort;//终止进程
	}
	int i = 0;
	while (i < M->num)
	{
		scanf("%d%d%d", &row, &col, &value);
		//若输入三元组行标/列标小于0或超过最大上限，判定为非法输入
		if (row < 0 || col < 0 || row > row_n || col > col_n)
		{
			printf("非法输入\n");
			abort;
		}
		else
		{
			M->data[i].row = row;
			M->data[i].col = col;
			M->data[i].value = value;
		}
		if (row > M->row_num)M->row_num = row;//设置总行数
		if (col > M->col_num)M->col_num = col;//设置总列数
		i++;
	}
	return M;
}
/*打印指定矩阵*/
void printMatrix(Matrix m)
{
	if (m->num)//若非零元素个数不为零，分行打印三元组元素
	{
		int i;
		for (i = 0; i < m->num; i++)
			printf("%d %d %d\n", m->data[i].row, m->data[i].col, m->data[i].value);
	}
	else
		printf("Empty.\n");
}
/*判断加法或运算是否合法并显示原因，合法则返回1*/
int ifAdd(Matrix& a, Matrix& b)
{
	if (a->row_num == b->row_num && a->col_num == b->col_num)
		return 1;//若矩阵A和B行列数分别相等，则A+B合法，返回值1
	else
		return 0;
}
/*判断乘法运算是否合法并显示原因*/
int ifMultiply(Matrix& a, Matrix& b)
{
	if (a->col_num == b->row_num)
		return 1;//若矩阵A列数和矩阵B行数相等，则AB合法，返回值1
	else if (a->row_num == b->col_num)
		return 2;//若矩阵A行数和矩阵B列数相等，则BA合法，返回值2
	else
		return 0;
}
/*插入一个三元组*/
void insert(Matrix m, triple t)
{
	m->data[m->num].row = t.row;//尾插
	m->data[m->num].col = t.col;
	m->data[m->num].value = t.value;
	m->num++;//更新非零元素个数
	if (t.row > m->row_num)m->row_num = t.row;//更新总行数
	if (t.col > m->col_num)m->col_num = t.col;//更新总列数
}
/*加法函数，完成矩阵加法，返回结果表*/
Matrix add(Matrix a, Matrix b)
{
	Matrix c = createMatrix(0, 0, 0);
	int i, j;
	for (i = 0, j = 0; i < a->num && j < b->num;)
	{
		if (a->data[i].row > b->data[j].row)//比较行号
			insert(c, b->data[j++]);
		else if (a->data[i].row < b->data[j].row)
			insert(c, a->data[i++]);
		else//行号相同
		{
			if (a->data[i].col > b->data[j].col)//比较列号
				insert(c, b->data[j++]);
			else if (a->data[i].col < b->data[j].col)
				insert(c, a->data[i++]);
			else if (a->data[i].value + b->data[j].value == 0)//下标相同，相加为0时
			{
				i++; j++;
			}
			else//和不为0
			{
				insert(c, a->data[i++]);
				c->data[c->num - 1].value += b->data[j++].value;
			}
		}
	}
	while (i < a->num)insert(c, a->data[i++]);
	while (j < b->num)insert(c, b->data[j++]);
	return c;
}
/*乘法函数，完成矩阵乘法，返回结果表*/
Matrix multiply(Matrix a, Matrix b)
{
	Matrix c = createMatrix(0, 0, 0);
	int i, j;
	int r_index = 0, c_index = 0;//r_index,c_index分别记录结果矩阵中元素行标、列标
	int a_index = 0;//记录矩阵A三元组表中元素下标
	int count = 0;
	while (a_index < a->num)
	{
		if (a->data[a_index].row > r_index++);//比较行标
		else if (a->data[a_index++].row < r_index);
		else//行标相同
		{
			for (i = a_index; a->data[i].row == r_index; i++)//当矩阵A中元素行标大于r_index时跳出循环
			{
				int value = 0;
				for (j = 0; j < b->num; j++)
				{
					if (b->data[j].col == r_index && b->data[j].row == a->data[i].col
						&& (a->data[i].value * b->data[j].value) != 0)
						//当A中元素a行标=B中元素b列标，a列标=b行标，且乘积不为0时
						value += a->data[i].value * b->data[j].value;
				}
				if (value)//若所得值不为0，则插入三元组
				{
					insert(c, a->data[i]);
					c->data[c->num - 1].col = c_index++;
					c->data[c->num - 1].value *= b->data[j].value;
					count++;
				}

			}
		}
	}
	return c;
}
/*工作函数，完成计算*/
void work()
{
	int row, col, num;
	printf("请输入矩阵A的行数、列数、非零元素个数：\n");
	scanf("%d%d%d", &row, &col, &num);
	printf("请输入矩阵A（以三元组表的形式）：\n");
	Matrix A = createMatrix(row, col, num);
	printf("请输入矩阵B的行数、列数、非零元素个数：\n");
	scanf("%d%d%d", &row, &col, &num);
	printf("请输入矩阵B（以三元组表的形式）：\n");
	Matrix B = createMatrix(row, col, num);
	if (ifAdd(A, B))//若A+B合法
	{
		Matrix C1 = add(A, B);
		printf("矩阵A和矩阵B相加结果:\nC1 = \n");
		printMatrix(C1);
		::free(C1);
	}
	else
		printf("矩阵A和矩阵B无法相加\n");
	Matrix C2;
	switch (ifMultiply(A, B))
	{
	case 1://若AB合法
		C2 = multiply(A, B);
		printf("矩阵A和矩阵B相乘结果:\nC2 = \n");
		::free(C2);
		break;
	case 2://若BA合法
		C2 = multiply(B, A);
		printf("矩阵B和矩阵A相乘结果:\nC2 = \n");
		::free(C2);
		break;
	default:
		printf("矩阵A和矩阵B无法相乘");
		break;
	}
	::free(A);//释放空间
	::free(B);
}
/*主函数*/
int main(void)
{
	work();

	return 0;
}