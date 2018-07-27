#include<iostream>
#include<fstream>
#include<vector>
#include<string>

#include"Util.h"

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
	TNode* subnodes[2];
	bool isleaf;
};
vector<TNode*> forest;
char line[1000];

bool isLeaf(string record) {
	return record.find("leaf")!=string::npos;
}
TNode* paserString(string record) {
	TNode* node=new TNode();
	node->isleaf = isLeaf(record);
	if (node->isleaf) 
	{
		//叶子节点
		node->value = atof(Util::split(Util::split(Util::split(record, ":")[1], ",")[0], "=")[1].c_str());
		node->id = atoi(Util::split(record, ":")[0].c_str());
	}
	else
	{
		//非叶子节点
		size_t pos1 = record.find_first_of(':');
		size_t pos2 = 0;
		size_t len = record.length();
		//取标号
		node->id = atoi(record.substr(0, len - pos1).c_str());
		//取阈值
		pos1 = record.find_first_of(']');
		pos2 = record.find_first_of('<') ;
		node->value = atof(record.substr(pos2+1, pos1 - pos2+1).c_str());
		//取索引号
		pos1 = record.find_first_of('f');
		node->index = atof(record.substr(pos1+1, pos2 - pos1+1).c_str());
		//取分支标号
		vector<string> v = Util::split(record, " ");
		vector<string> kv;
		v = Util::split(v[1], ",");
		for (string s : v)
		{
			kv = Util::split(s, "=");
			if (kv[0] == "yes") node->yes = atoi(kv[1].c_str());
			if (kv[0] == "no") node->no = atoi(kv[1].c_str());
			if (kv[0] == "missing") node->missing = atoi(kv[1].c_str());
		}
	}
	node->lnode = nullptr;
	node->rnode = nullptr;
	return node;
}
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
	if (n->isleaf) return;
}

float testdata[5000];

float searchTree(TNode* root,float* data) {
	//递归终止
	if (root->isleaf) return root->value;

	float value=0.0;
	if (abs(data[root->index])< 1e-6 ) {
		//missing
		if (root->lnode->id == root->missing)
			value=searchTree(root->lnode, data);
		else if (root->rnode->id == root->missing)
			value=searchTree(root->rnode, data);
		else
			cout << "匹配missing节点异常" << endl;
	}
	else if (data[root->index] < root->value) {
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
	ifstream f("md.txt");
	ifstream t("test.txt");
	ofstream r("result.txt");
	if (!f.is_open()) {
		cout << "打开数据文件失败" << endl;
		return 0;
	}

	if (!t.is_open()) {
		cout << "打开测试文件失败" << endl;
		return 0;
	}
	
	//建树
	while (!f.eof()) {
		f.getline(line, 1000);
		if (line[0] == 0) continue;
		TNode* n = paserString(line);
		if (n->id == 0) forest.push_back(n);
		buildTree(n, f);
	}

	//读取test数组
	float sum;
	while (!t.eof()) {
		sum = 0.0;
		for (int i = 0; i < 5000; ++i) {
			t >> testdata[i];
		}
		for (auto i : forest) {
			sum += searchTree(i,testdata);
		}
		r << sum << endl;
	}

	system("pause");
	return 0;
}