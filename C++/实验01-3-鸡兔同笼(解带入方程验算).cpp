#include<iostream>
using namespace std;
int main()
{
    int x,y,mm,nn;

    cin>>x>>y;
    nn=(y-2*x)/2;
    mm=x-nn;
    if(mm>=0&&nn>=0&&y==2*mm+4*nn)
    	cout<<"chickens = "<<mm<<"; "<<"rabbits = "<<nn<<endl;
    else
    	cout<<"No solution!"<<endl;
    return 0;
}
