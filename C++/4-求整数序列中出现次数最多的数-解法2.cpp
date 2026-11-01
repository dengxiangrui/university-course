#include <iostream>
#include <vector>
#include <algorithm> //包含排序函数sort() 
using namespace std;

int main()
{
	int N;
	cin>>N;
	vector<int> vec(N+1);
		
	for(int i=0;i<N;i++)
		cin>>vec[i];
	sort(vec.begin(),vec.begin()+N-1);//vector元素排序 
	int startIdx=0;//记录排序后数值相同子序列的首下标 
	int count=0,maxCount=0,maxCountVal=vec[0];
	vec[N]=vec[N-1]+1;//在末尾元素后面设置：岗哨元素!=末尾元素 
	for(int i=0;i<N;i++){
		if(vec[i]!=vec[i+1]){//当前相同整数值子序列的下标为startIdx到i 
			count=i-startIdx+1;//vec[i]出现次数=子序列长度 				
			if(count>maxCount){
				maxCount=count;
				maxCountVal=vec[i];
			}
			startIdx=i+1;	
		}		
	}
	cout<<maxCountVal<<' '<<maxCount;
	return 0;
}
