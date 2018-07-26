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

float compute() {


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
	while (!t.eof()) {
		for (int i = 0; i < 5000; ++i) {
			t >> testdata[i];
		}

	}


	//TNode n = paserString("123:[f728<0.468949] yes=1,no=2,missing=2,gain=1.71789e+06,cover=1.08553e+07");
	//if (n.id == 0) forest.push_back(n);
	//buildTree(n,)

	system("pause");
	return 0;
}