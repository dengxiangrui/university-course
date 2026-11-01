#include <iostream> 
using namespace std;

int main()
{ 
	//整型数组 
	const int SIZE=100; 
	int array_int[SIZE]={10,20,30,40,50};//定义大小为100个元素的数组，并赋部分初始值 
	int i;
	for(i=0;i<10;i++)	//输出初始化后数组的前10个元素 
		cout<<array_int[i]<<' ';
	cout<<endl<<"输入5个元素值：";
	for(i=0;i<5;i++) 
		cin>>array_int[i];//输入5个元素
	cout<<"数组前5个元素：";
	for(i=0;i<5;i++) 
		cout<<array_int[i]<<' ';//输出数组前5个元素 
	cout<<endl<<"逆序输出数组元素：";
	for(i=4;i>=0;i--) 
			cout<<array_int[i]<<' ';//逆序输出数组前5个元素
				
	//查找数组元素 	
	cout<<endl<<"数组元素个数：";
	int n,key,found=0;
	/*设found为查找标志：
	found==1（真）时表示找到与键值相等的数组元素；
	found==0时表示未找到*/ 
	cin>>n;
	cout<<"输入"<<n<<"个数组元素："<<endl; 
	for(i=0;i<n;i++)
		cin>>array_int[i];
	cout<<"输入需查找的键值： ";
	cin>>key;
	for(i=0;i<n;i++) 
		if(array_int[i]==key){found=1;break;}
	if(found)//if(found==1)	
		cout<<"Index of key is "<<i;
		else cout<<"not found" <<endl;
		
	//输入若干（不超过100）数组元素，以-1结束
	i=0;
	cout<<endl<<"输入若干（不超过100）数组元素，以-1结束："<<endl;
	int item;
	while(i<100)
	{
		cin>>item;
		if(item==-1) break;
		array_int[i++]=item;
	}		
	n=i;	
	for(i=0;i<n;i++)
		cout<<array_int[i]<<' ';
	return 0; 
}
