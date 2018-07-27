#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

//节点结构体
struct TNode {
	int id;
	int index;
	float value;
	int yes;
	int no;
	int missing;
	TNode* lnode;
	TNode* rnode;
	bool isleaf;
};

//全局变量
vector<TNode*> forest;
char line[1000];
float testdata[5000];

//解析字符串
TNode* paserString(string record) {
	TNode* node=new TNode();
	node->lnode = nullptr;
	node->rnode = nullptr;

	node->isleaf = record.find('l') != string::npos;

	if (node->isleaf) {
		sscanf_s(record.c_str(), "%d:leaf=%f",	&node->id,  &node->value);
	}
	else {
		sscanf_s(record.c_str(), "%d:[f%d<%f] yes=%d,no=%d,missing=%d",
			&node->id, &node->index, &node->value, &node->yes, &node->no, &node->missing);
	}
	
	return node;
}

//递归建树函数
void buildTree(TNode* root, ifstream& f)
{
	f.getline(line, 1000);
	TNode* n = paserString(line);

	if (n->id == root->yes) 
	{
		root->lnode = n;
		if(!n->isleaf) buildTree(n, f);
	}

	f.getline(line, 1000);
	n = paserString(line);
	if (n->id == root->no) 
	{
		root->rnode = n;
		if (!n->isleaf) buildTree(n, f);
	}
	return;
}


//递归检索函数
float searchTree(TNode* root,float* data) {
	//递归终止条件
	if (root->isleaf) return root->value;

	float value=0.0;
	if (fabs(data[root->index])< 1e-6 ) {
		//missing
		cout << "missing " <<data[root->index]<< endl;
		if (root->lnode->id == root->missing)
			value=searchTree(root->lnode, data);
		else if (root->rnode->id == root->missing)
			value=searchTree(root->rnode, data);
		else
			cout << "匹配missing节点异常" << endl;
	}
	else if (data[root->index] < root->value && fabs(root->value- data[root->index])>1e-6 ) {
		//yes
		if (root->lnode->id == root->yes)
			value=searchTree(root->lnode, data);
		else if (root->rnode->id == root->yes)
			value=searchTree(root->rnode, data);
		else
			cout << "匹配yes节点异常" << endl;
	}
	else {
		//no
		if (root->lnode->id == root->no)
			value=searchTree(root->lnode, data);
		else if (root->rnode->id == root->no)
			value=searchTree(root->rnode, data);
		else
			cout << "匹配no节点异常" << endl;
	}
	return value;
}

int main() 
{

	cout << "==========================================" << endl;
	cout << "               使用说明                   " << endl;
	cout << "1.读取当前文件夹下的md.txt创建数据索引      " << endl;
	cout << "2.读取当前文件夹下的test.txt作为测试数据源  " << endl;
	cout << "3.测试数据源数组元素使用空格隔开            " << endl;
	cout << "4.测试结果输出到result.txt中               " << endl;
	cout << "==========================================" << endl;


	ifstream f("md.txt");
	ifstream t("test.txt");
	ofstream r("result.txt");
	if (!f.is_open()) {
		cout << "打开数据文件md.txt失败" << endl;
		return 0;
	}

	if (!t.is_open()) {
		cout << "打开测试文件test.txt失败" << endl;
		return 0;
	}
	
	cout << "创建索引树" << endl;
	//建树
	while (!f.eof()) {
		f.getline(line, 1000);
		if (line[0] == 0) continue;
		TNode* n = paserString(line);
		if (n->id == 0) forest.push_back(n);
		buildTree(n, f);
	}

	//读取test数组
	cout << "读取测试数据" << endl;
	float sum;
	while (!t.eof()&&t.peek()!=EOF) {
		sum = 0.0;
		memset(testdata, 0, sizeof(testdata));
		for (int i = 0; i < 5000; ++i) {
			t >> testdata[i];
		}
		if (t.eof()) break;
		for (auto i : forest) {
			sum += searchTree(i,testdata);
		}
		cout << sum << endl;
		r << sum << endl;
	}

	system("pause");
	return 0;
}