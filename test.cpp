#include<iostream>
#include<vector>
using namespace std;

int main()
{
	vector<int*> vec;
	int x = 10;
	vec.push_back(&x);
	cout << *vec[0];
	return 0;	
}
