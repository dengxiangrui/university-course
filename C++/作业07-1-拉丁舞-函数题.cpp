#include <iostream>
#include <string>
using namespace std;
const int K=2;
const int N=20;
class Student{
  string name;
//对象指针数组，数组的每个元素welcome[i]是对象指针
  Student *welcome[K];//对象指针数组-期待的两位舞伴
  Student *pair;//对象指针
  public:
      void init(string &name, Student *a, Student *b) {
        this->name=name; 
        welcome[0]=a;
        welcome[1]=b;
        pair=NULL;//配对舞伴指针
      }
     void printPair();
     void addPair();        
};

/* 请在这里填写答案 */
void Student::printPair(){
	if(pair!=NULL)
		cout<<name<<":" <<pair->name<<endl;
}
void Student::addPair(){	
	if(pair!=NULL) return;//容易忽略出错-避免按女生进行匹配时重复配对 
	Student *myPair=NULL;
	for(int i=0;i<2;i++){		
		myPair=welcome[i];
		if(myPair->pair==NULL){//this为指向当前对象的指针 
			if((myPair->welcome[0]==this||myPair->welcome[1]==this)
			 ||(myPair->welcome[0]->pair!=NULL&&myPair->welcome[1]->pair!=NULL)){
				pair=myPair;
				myPair->pair=this;
				break;
			}
		}	
	}
}    

int main(){
    Student male[N], female[N];
    int m, f, i, j, a, b;
    string name;
    cin>>m;
    for(i=0;i<m;i++){
      cin>>name>>a>>b;
      male[i].init(name, &female[a], &female[b]);
    }
    cin>>f;
    for(i=0;i<f;i++){
      cin>>name>>a>>b;
      female[i].init(name, &male[a], &male[b]);
    }
    for(i=0;i<m;i++) male[i].addPair();
    for(i=0;i<f;i++) female[i].addPair();
    for(i=0;i<m;i++) male[i].printPair();
    return 0;
}
