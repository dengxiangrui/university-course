#include <iostream>
#include <iomanip>
using namespace std;

int main(){
	int n;
	cin>>n;
	cout<<hex //输出16进制数 
		<< setw(8) << setfill('0') <<n<<endl;
	
	unsigned char* pn=NULL;
	pn=(unsigned char*)&n;
	cout<<hex
		<< setw(2) << setfill('0') <<(int)*pn<<' '
		<< setw(2) << setfill('0') <<(int)*(pn+1) <<' '
		<< setw(2) << setfill('0') <<(int)*(pn+2)<<' '
		<< setw(2) << setfill('0') <<(int)*(pn+3);
	return 0;
}
