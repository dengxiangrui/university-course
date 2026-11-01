#include <iostream>
#include <string>
using namespace std;

int main()
{
    string str;
    getline(cin,str);
    char c;
    //cin>>c; //cin>>进行输入时会把空格作为间隔符忽略掉
	c=cin.get(); //可读入空格 
    int count= 0;
    for(char ch : str)
    //for(int i=0;i<str.size();i++) 
    {
        if(ch==c)
        //if(str[i]==c)
        {
            count++;
        }
    }
    cout<<count;
    return 0;
}
