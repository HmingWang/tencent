#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

int main() {

	//生成测试文件
	ofstream f("test.txt");
	srand(time(0));
	float a;
	float b;
	float c;
	//生成10行，每行5000个浮点数
	int n = 10;
	while (n--) {
		int m = 5000;
		while (m--) {
			a = rand()*1.0;
			b = rand()*1.0;
			c = rand();
			if (c==0) f << 0<<' ';
			else 
				f << (a - b) / c<<' ';
		}

		f << endl;
	}


	return 0;
}