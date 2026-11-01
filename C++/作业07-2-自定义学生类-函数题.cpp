class Student
{
private:
    int m_id;
    char m_name[10];
public:
    Student(int id=0,char *name="");
    ~Student();
    void print();
};

#include <iostream>
#include <cstring>
using namespace std;

int main()
{
    Student stu_array[3]={Student(1,"Zhang"),Student(2,"Wang")};
    return 0;
}

/* 请在这里填写答案 */
//构造函数要完成两个数据成员的初始赋值，并做一行输出，形如："Hi!学号 姓名"。
Student::Student(int id,char *name){
	m_id=id;
	strcpy(m_name,name);//复制字符串 
	cout<<"Hi!"<<m_id<<" "<<m_name<<endl;
}

//析构函数要求输出一行，形如："Bye!学号 姓名"。
Student::~Student(){
	cout<<"Bye!"<<m_id<<" "<<m_name<<endl;
}

//print函数要求输出一行，形如："Bye!学号 姓名"
void Student::print(){
	cout<<"Bye!"<<m_id<<" "<<m_name<<endl;
}
