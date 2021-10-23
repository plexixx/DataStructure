#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include<fstream>

using namespace std;

typedef struct Huffmantree //哈夫曼树
{
	char data;//所存字符
	long long weight;//权重
	int ifl;//回溯标志
	Huffmantree* lc;//左孩子
	Huffmantree* rc;//右孩子
	Huffmantree* parents;//双亲
}ht;

typedef struct Codelist//编码表
{
	char letter;//字符
	vector<bool>code;//对应编码
}codelist;

typedef struct Forest//建树时用到的森林（采用双向链表存储）
{
	ht* data;//对应树
	struct Forest* pre;//前驱
	struct Forest* next;//后继
}forest;

vector<char> readfile()//将文件按照二进制方式，以字节为单位读入
{
	fstream openfile;
	string filename;
	cout << "请输入文件路径（包含文件名）" << endl;
	cin >> filename;
	openfile.open(filename, ios::in | ios::binary);
	vector<char> originaldata;
	while (1)
	{
		char now;
		openfile.read(&now, sizeof(char));
		if (openfile.eof())	break;
		originaldata.push_back(now);
	}
	return originaldata;
}

deque<bool>readfileasb(fstream& openfile)//将文件以二进制方式，以每一个二进制位为单位读入缓冲区
{
	deque<bool>buffer;
	while (1)
	{
		char word;
		unsigned char uword;
		openfile.read(&word, sizeof(char));
		if (openfile.eof())	break;
		uword = (unsigned char)word;
		for (int i = 1; i <= 8; i++)//将读入的八位的字节拆分为单独的二进制位存储
		{
			buffer.push_back(uword / 128);
			uword = uword << 1;
		}
	}
	int excess = 0;
	int j = 1;
	for (int i = 1; i <= 8; i++)//根据文件结尾预留信息，修正文件结尾数据
	{
		excess += j * buffer.back();
		j *= 2;
		buffer.pop_back();
	}
	if (excess)
		for (int i = 1; i <= excess; i++)buffer.pop_back();
	return buffer;
}

void writefile(queue<bool>buffer, fstream& wd)//将缓冲区中的内容写入文件
{
	unsigned char word = 0;
	int wordlength = 0;
	while (!buffer.empty())
	{
		word = buffer.front() + (word << 1);
		wordlength++;
		buffer.pop();
		if (wordlength == 8)//每八位整合为一个字节存入
		{
			wd.write((const char*)&word, sizeof(char));
			wordlength = 0;
			word = 0;
		}
	}
	if (wordlength != 0)//修正文件结尾
	{
		word = word << (8 - wordlength);
		wd.write((const char*)&word, sizeof(char));
	}
	word = (8 - wordlength) % 8;//结尾添加修正信息
	wd.write((const char*)&word, sizeof(char));
	return;
}

forest* creatforest()//创建一个森林，用于构建哈夫曼树
{
	forest* mq = (forest*)malloc(sizeof(forest));
	mq->pre = mq;
	mq->next = NULL;
	return mq;
}

void addtreetoforest(forest* f, ht* t)//向森林中添加树
{
	f->pre->next = (forest*)malloc(sizeof(forest));
	f->pre->next->pre = f->pre;
	f->pre->next->data = t;
	f->pre->next->next = NULL;
	f->pre = f->pre->next;
	return;
}

ht* deltreeinforest(forest* f, forest* tree)//从森林中删除某颗树
{
	ht* t = tree->data;
	if (tree->next)
	{
		tree->pre->next = tree->next;
		tree->next->pre = tree->pre;
	}
	else
	{
		f->pre = tree->pre;
		tree->pre->next = NULL;
	}
	return t;
}

forest* findmin(forest* f)//找到森林中的最轻的树，如果权重相等，则取最小叶子的char值最小的树
{
	forest* mintree = f->next;
	forest* now = f->next;
	while (now)//遍历森林
	{
		if (now->data->weight < mintree->data->weight)
		{
			mintree = now;
		}
		else if (now->data->weight == mintree->data->weight)
		{
			if (mintree->data->data > now->data->data)
			{
				mintree = now;
			}
		}
		now = now->next;
	}
	return mintree;
}

vector<ht>creatweightlist(vector<char>originaldata)//根据原始序列创建权重列表
{
	vector<ht>dw(257);
	for (int i = 0; i <= 255; i++)
	{
		dw[i].data = i - 128;
		dw[i].weight = 0;
	}
	for (int i = 0; i < originaldata.size(); i++)
	{
		dw[(int)originaldata[i] + 128].weight += 1;
		//dw[(int)originaldata[i] + 128].weight %= 256;//将权值范围压缩至0-255
	}
	return dw;
}

vector<ht>creatweightlistbyread(fstream& rw)//从压缩文件中读入权值表
{
	vector<ht>wl(256);
	for (int i = 0; i <= 255; i++)
	{
		rw.read(&(wl[i].data), sizeof(char));
		long long noww;
		rw.read((char*)&noww, sizeof(long long));
		wl[i].weight = noww;
	}
	return wl;
}

ht* creathuffmantreebyweightlist(vector<ht>dw)//根据权重列表创建树
{
	forest* f = creatforest();
	for (int i = 0; i <= 255; i++)//将树全部放入森林
	{
		ht* now = (ht*)malloc(sizeof(ht));
		*now = dw[i];
		addtreetoforest(f, now);
	}
	ht* tree = NULL;
	while (f->next)
	{
		forest* nowtree1 = findmin(f);//取最小
		deltreeinforest(f, nowtree1);
		if (f->next == NULL) { tree = nowtree1->data; break; }//如果森林中只剩一棵树，则跳出循环
		forest* nowtree2 = findmin(f);//取次小
		deltreeinforest(f, nowtree2);
		ht* newtree = (ht*)malloc(sizeof(ht));
		newtree->data = nowtree1->data->data;
		newtree->lc = nowtree1->data;
		newtree->rc = nowtree2->data;
		nowtree1->data->parents = newtree;
		nowtree2->data->parents = newtree;
		newtree->weight = nowtree1->data->weight + nowtree2->data->weight;
		addtreetoforest(f, newtree);
		free(nowtree1);//释放空间
		free(nowtree2);
	}
	return tree;
}

void writeweightlist(vector<ht>wl, fstream& wt)//将权重列表写入文件作为文件编码表
{
	for (int i = 0; i <= 255; i++)
	{
		wt.write(&(wl[i].data), sizeof(char));
		long long noww = wl[i].weight;
		wt.write((char*)&noww, sizeof(long long));
	}
	return;
}

void resetifl(ht* t)//初始化树上各点的回溯状态
{
	if (!t)return;
	t->ifl = 0;
	resetifl(t->lc);
	resetifl(t->rc);
	return;
}

vector<codelist> creatcodelist(ht* huffmantree)//根据哈夫曼树，创建编码表
{
	resetifl(huffmantree);
	vector<codelist>list(256);
	vector<bool>code;
	ht* now = huffmantree;
	now->ifl = 0;
	int flag = 1;//循环跳出标志
	while (flag)
	{
		while (now->ifl == 2)//状态2：树的左右孩子都遍历过，回溯
		{
			if (now == huffmantree) { flag = 0; break; }//回到根则遍历结束
			now = now->parents;
			code.pop_back();
		}
		if (now->ifl == 0)//状态0：树的左右孩子都没有被遍历过
		{
			if (now->lc)
			{
				code.push_back(0);
				now->ifl = 1;
				now = now->lc;
			}
			else if (now->rc)
			{
				code.push_back(1);
				now->ifl = 2;
				now = now->rc;
			}
			else//左右为空，说明为叶子，存储路径，回溯
			{
				list[((unsigned)now->data) % 256].letter = now->data;
				list[((unsigned)now->data) % 256].code = code;
				now = now->parents;
				code.pop_back();
			}
		}
		else if (now->ifl == 1)//状态1：树的左孩子都被遍历过，右孩子尚未遍历
		{
			if (now->rc)
			{
				code.push_back(1);
				now->ifl = 2;
				now = now->rc;
			}
			else
			{
				now->ifl = 2;
			}
		}
	}
	return list;
}

queue<bool> writedata(ht* huffmantree, vector<char>originaldata)//根据原数据和构建好的哈夫曼树，将编码写入文件
{
	queue<bool> buffer;
	vector<codelist> list = creatcodelist(huffmantree);
	char word = 0;
	int wordlength = 1;
	for (int i = 0; i < originaldata.size(); i++)
	{
		int k;
		for (k = 0; k <= 255; k++)//对于每个字节的数据，找到其对应的编码
		{
			if (list[k].letter == originaldata[i])break;
		}
		for (int j = 0; j < list[k].code.size(); j++)//将对应编码存入缓冲区
		{
			buffer.push(list[k].code[j]);
		}
	}
	return buffer;
}

void compressfile()//实现压缩文件功能
{
	vector<char>originaldata = readfile();//读入原数据
	vector<ht>dw = creatweightlist(originaldata);//创建权重表
	ht* huffmantree = creathuffmantreebyweightlist(dw);//创建哈夫曼树
	string filename;//读入输出路径
	cout << "请输入输出文件路径（包含文件名）" << endl;
	cin >> filename;
	fstream wf;
	wf.open(filename, ios::out | ios::binary);//创建输出文件
	writeweightlist(dw, wf);//将权重表写入文件
	queue<bool>buffer;
	buffer = writedata(huffmantree, originaldata);//获取数据写入缓冲区数据
	writefile(buffer, wf);//将缓冲区数据写入文件
	return;
}

void uncompressfile()//实现解压文件功能
{
	string filename;
	cout << "请输入文件路径（包含文件名）" << endl;
	cin >> filename;
	fstream rd;//读入压缩文件
	rd.open(filename, ios::in | ios::binary);
	cout << "请输入输出文件路径（包含文件名）" << endl;
	cin >> filename;
	fstream wd;//读入输出文件
	wd.open(filename, ios::out | ios::binary);//创建输出文件
	vector<ht>wl = creatweightlistbyread(rd);//读入权重表
	ht* htree = creathuffmantreebyweightlist(wl);//根据权重表建立哈夫曼树
	deque<bool>bufferin;
	bufferin = readfileasb(rd);//将数据转换为单个二进制位的形式，存入缓冲区
	ht* now = htree;
	while (1)//根据数据还原原文件
	{
		if (now->lc || now->rc)//如果没有走到叶子，继续走，0为向左走，1为向右走
		{
			int codenow = bufferin.front();
			bufferin.pop_front();
			if (codenow)now = now->rc;
			else now = now->lc;
			if (now->lc == NULL && now->rc == NULL)//如果走到叶子，还原数据，当前指针回至根
			{
				wd.write(&(now->data), sizeof(char));
				now = htree;
			}
		}
		if (bufferin.empty())break;
	}
	return;
}

void work()//总工作函数
{
	char useroperator;
	cout << "欢迎使用，请输入c以压缩文件，输入u以解压缩文件,输入q退出" << endl;
	cin >> useroperator;
	while (useroperator != 'q')//根据用户的命令执行压缩、解压或退出操作
	{
		if (useroperator == 'c')
		{
			compressfile();
			cout << "压缩成功" << endl;
			system("pause");
			break;
		}
		else if (useroperator == 'u')
		{
			uncompressfile();
			cout << "解压成功" << endl;
			system("pause");
			break;
		}
		else
		{
			cout << "无效的命令！" << endl;
			cout << "请重新输入" << endl;
			cin >> useroperator;
		}
	}
	return;
}

int main()//总调度函数
{
	work();
	return 0;
}

//G:\大学用相关文件\数据结构\实验\实验4\test123.txt
//C:\Users\byx10\source\repos\ConsoleApplication11\test1.txt
//C:\Users\byx10\source\repos\ConsoleApplication11\xxx.555