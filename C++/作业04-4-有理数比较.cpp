#include <iostream>
using namespace std;

int main()
{
	struct fraction{	//分数  
		int num;	//分子 
		int deno;	//分母 
	} f1,f2;
	
	char ch; 
	
	cin>>f1.num;cin>>ch;cin>>f1.deno;//自动跳过空格 ，读下一个整数 
	cin>>f2.num;cin>>ch;cin>>f2.deno;
	
	if(f1.num*f2.deno>f2.num*f1.deno) 
		cout<<f1.num<<'/'<<f1.deno<<" > "<<f2.num<<'/'<<f2.deno<<endl;
	if(f1.num*f2.deno==f2.num*f1.deno) 
		cout<<f1.num<<'/'<<f1.deno<<" = "<<f2.num<<'/'<<f2.deno<<endl;
	if(f1.num*f2.deno<f2.num*f1.deno) 
		cout<<f1.num<<'/'<<f1.deno<<" < "<<f2.num<<'/'<<f2.deno<<endl;
	return 0;
}
