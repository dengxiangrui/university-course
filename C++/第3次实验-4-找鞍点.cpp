#include <iostream>
using namespace std;
 
int main(){
    const int SIZE=6;
    int arr[SIZE][SIZE];
    int n;
    cin>>n;
    /*
	 1.C++标准规定，数组的大小(数组维度)
	 必须使用常量（如数字或const修饰的常量）定义。
	 2.部分编译器（如DEVCPP）支持非标准扩展，
	 允许用变量定义数组长度如下，但这种用法不具有普遍性且存在风险。 
	  int arr[n][n];
	*/ 
	
    for(int i=0;i<n;i++){ //输入矩阵元素 
        for(int j=0;j<n;j++){
            cin >> arr[i][j];
        }
    }
    
    int rowMax,flag=0; //flag标记是否找到鞍点  
    for(int i=0;i<n;i++){
    	rowMax=arr[i][0]; 
        for(int j=0;j<n;j++){//查找第i行最大值，注意可能存在多个 
            if(rowMax<arr[i][j]) rowMax=arr[i][j];
		}
		
		int row=0;
		flag=0;
		for(int j=0;j<n;j++){//逐个检测i行元素 
		    if(arr[i][j]==rowMax){//i行中最大值
		    	for(row=0;row<n;row++){//是否该列最小？ 
					if(arr[row][j]<rowMax) break;//不是该列最小 
				}
				if(row==n){//是该列最小，已找到鞍点标记并输出 
					flag=1;
					cout<<i<<' '<<j;
					return 0;	//题目保证给出的矩阵至多存在一个鞍点
				} 
		    }
		}
    }
		
	if(!flag) cout<<"NONE";
}
