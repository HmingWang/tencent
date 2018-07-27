#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

//�ڵ�ṹ��
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

//ȫ�ֱ���
vector<TNode*> forest;
char line[1000];
float testdata[5000];

//�����ַ���
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

//�ݹ齨������
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


//�ݹ��������
float searchTree(TNode* root,float* data) {
	//�ݹ���ֹ����
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
			cout << "ƥ��missing�ڵ��쳣" << endl;
	}
	else if (data[root->index] < root->value && fabs(root->value- data[root->index])>1e-6 ) {
		//yes
		if (root->lnode->id == root->yes)
			value=searchTree(root->lnode, data);
		else if (root->rnode->id == root->yes)
			value=searchTree(root->rnode, data);
		else
			cout << "ƥ��yes�ڵ��쳣" << endl;
	}
	else {
		//no
		if (root->lnode->id == root->no)
			value=searchTree(root->lnode, data);
		else if (root->rnode->id == root->no)
			value=searchTree(root->rnode, data);
		else
			cout << "ƥ��no�ڵ��쳣" << endl;
	}
	return value;
}

int main() 
{

	cout << "==========================================" << endl;
	cout << "               ʹ��˵��                   " << endl;
	cout << "1.��ȡ��ǰ�ļ����µ�md.txt������������      " << endl;
	cout << "2.��ȡ��ǰ�ļ����µ�test.txt��Ϊ��������Դ  " << endl;
	cout << "3.��������Դ����Ԫ��ʹ�ÿո����            " << endl;
	cout << "4.���Խ�������result.txt��               " << endl;
	cout << "==========================================" << endl;


	ifstream f("md.txt");
	ifstream t("test.txt");
	ofstream r("result.txt");
	if (!f.is_open()) {
		cout << "�������ļ�md.txtʧ��" << endl;
		return 0;
	}

	if (!t.is_open()) {
		cout << "�򿪲����ļ�test.txtʧ��" << endl;
		return 0;
	}
	
	cout << "����������" << endl;
	//����
	while (!f.eof()) {
		f.getline(line, 1000);
		if (line[0] == 0) continue;
		TNode* n = paserString(line);
		if (n->id == 0) forest.push_back(n);
		buildTree(n, f);
	}

	//��ȡtest����
	cout << "��ȡ��������" << endl;
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