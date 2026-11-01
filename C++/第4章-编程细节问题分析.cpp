#include<iostream>
using namespace std;
 /*
 1、注释 良好的程序设计风格：适当书写注释，提高程序可读性 
 1）单行注释 // 
 2）多行注释 
 3）调试程序，暂时跳过代码 
 
 2、变量命名问题
 1）命名尽量明确，有意义
 2）不必吝惜，也不要太多，太多太随意程序不好读 
 
 3、变量值初始化问题 能初始化最好初始化 
 */ 
 
int main(){
	int n;
	cin>>n;
	const int size=10;
	int num[size];
	for(int i=0;i<n;i++){
		cin>>num[i];
	}
	
	int minValue =num[0];//int s=num[0];
	/**minIdx是否一定要初始化或赋值为0?**/
	int minIdx=0;//int b,c;
	for(int i=1;i<n;i++)//{for(int i=0;i<n;i++){
		/**思考：为什么minIdx如果不给定初始值0，
		在这里写成for(int i=0;i<n;i++)if(num[i]<minValue){就运行错误，
		写成for(int i=0;i<n;i++)if(num[i]<=minValue){就正确？**/
		if(num[i]<minValue){//if(num[i]<s){
			minValue=num[i];//s=num[i];
			minIdx=i;//b=i;
		}
	}
	int temp;
	temp=num[0];//c=num[0];//最小元素与第一个元素（下标0）交换 
	num[0]=minValue;//num[0]=s;
	num[minIdx]=temp;//num[b]=c;
	
	
	/*以下同理*/
	int l=num[0];
	int a,d;
	for(int i=0;i<n;i++){
		if(num[i]>l){
			l=num[i];
			a=i;
		}
	}
	d=num[n-1];
	num[n-1]=l;
	num[a]=d;	
	for(int i=0;i<n;i++){
		cout<<num[i]<<" ";
	}
}
