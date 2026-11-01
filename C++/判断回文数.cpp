//ÅĞ¶Ï»ØÎÄÊı
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

bool huiwen (int dig)
{
	bool res = 1;
	vector<int> vec;
	vector<int> vec1;
	int temp;
	while(dig > 0)
	{
		temp = dig%10;
		vec.push_back(temp);
		vec1.push_back(temp);
		dig /= 10;
	}
	reverse(vec.begin(), vec.end());
	for(int i = 0; i < vec.size(); i++)
	{
		if(vec[i] != vec1[i])
		{
			res = 0;
			break;
		}
	}
	return res;
} 

int main()
{
	int num;
	cin >> num;
	cout<< huiwen(num);
}
