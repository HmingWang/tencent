#include<iostream>
#include<vector>
#include<string>

#include"CommFun.h"

using namespace std;

struct TNode {
	int num;
	float value;
	int yes;
	int no;
	int missing;
	TNode* lnode;
	TNode* rnode;
	bool isleaf;
};
vector<TNode> forest;
bool isLeaf(string record) {
	return record.find("leaf")!=string::npos;
}
TNode paserString(string record) {
	TNode node;
	node.isleaf = isLeaf(record);
	if (node.isleaf) 
	{
		//叶子节点
		node.value = atof(CommFun::split(CommFun::split(CommFun::split(record, ":")[1], ",")[1], "=")[1].c_str());
	}
	else
	{
		//非叶子节点
		size_t pos1 = record.find_first_of(':');
		size_t pos2 = 0;
		size_t len = record.length();
		//取标号
		node.num = atoi(record.substr(0, len - pos1).c_str());
		//取阈值
		pos1 = record.find_first_of(']');
		pos2 = record.find_first_of('<') + 1;
		node.value = atof(record.substr(pos2, pos1 - pos2).c_str());

		//取分支标号
		vector<string> v = CommFun::split(record, " ");
		vector<string> kv;
		v = CommFun::split(v[1], ",");
		for (string s : v)
		{
			kv = CommFun::split(s, "=");
			if (kv[0] == "yes") node.yes = atoi(kv[1].c_str());
			if (kv[0] == "no") node.no = atoi(kv[1].c_str());
			if (kv[0] == "missing") node.missing = atoi(kv[1].c_str());
		}
	}
	node.lnode = nullptr;
	node.rnode = nullptr;
	return std::move(node);
}
void buildTree(TNode& root, string record)
{
	//分析字符串
	//if (node.num == root.yes) root.lnode = &node;
	//if (node.num == root.no) root.rnode = &node;

}
int main() 
{



	TNode n = paserString("                    123:[f728<0.468949] yes=1,no=2,missing=2,gain=1.71789e+06,cover=1.08553e+07");
	if (n.num == 0) forest.push_back(n);
	//buildTree(n,)

	system("pause");
	return 0;
}