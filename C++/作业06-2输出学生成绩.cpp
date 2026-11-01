#include <iostream>
#include <iomanip>
using namespace std;

int main(){
	int N;
	cin>>N;
	double *pS=new double[N];
	
	for(int i=0;i<N;i++){
		cin>>*(pS+i);
	}
	
	double sum=0.0,avg,maxS,minS;
	maxS=minS=sum=*pS;
	for(int i=1;i<N;i++){
		double val=*(pS+i);
		sum+=val;
		if(val<minS) minS=val;
		if(val>maxS) maxS=val;
		} 
	avg=sum/N;
	cout<<fixed<<setprecision(2)<<"average = "<<avg<<endl
		<<"max = "<<maxS<<endl
		<<"min = "<<minS;
	
	delete []pS;
	return 0;
}
