#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    int N=0; 
    struct Record{
		int bookId;
		char key;
		int hh,mm;		
	}; 
	struct Record rec[10000];//当数组大小设置为1000时，提示段错误 
	
	cin>>N;
	int idx=0;
	int time=0,times=0;
	double avgTime=0;
	char ch;//ch用于读时间中的 ： 
	for(int day=0;day<N;day++) {
		idx=0;
		time=0;times=0;avgTime=0;
		while(true){//读1天的记录 
			cin>>rec[idx].bookId>>rec[idx].key>>rec[idx].hh>>ch>>rec[idx].mm;
			if(rec[idx].bookId==0) break;
			idx++;
		}	
		
		int id=0;
		char key;
		int j;
		for(int i=0;i<idx;i++){
			id=rec[i].bookId;
			key=rec[i].key;
			if(key=='S'){
				for(j=i+1;j<idx;j++){//查找同一本书在E之前连续出现的S 
					if(rec[j].bookId==id&&rec[j].key=='S') {//忽略前一条S记录
						j=idx;
						break;
					}
					if(rec[j].bookId==id&&rec[j].key=='E') {//查找匹配的E记录 
						rec[j].key='M';
						break;
					}					
				}
				if(j<idx){//统计1天的数据 
					times++;
					if(rec[j].hh==0 && rec[j].mm==0) rec[j].hh=24;
					time+=rec[j].hh*60+rec[j].mm-(rec[i].hh*60+rec[i].mm);
				}		
			}
		}
		if(times)//在times>0,有记录的情况下进行计算，否则输出0 
			avgTime=(double)time/times;
		cout<<times<<' '<<fixed<<setprecision(0)<<avgTime<<endl;
	}

    return 0;
}
