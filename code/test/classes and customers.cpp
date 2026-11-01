#include <iostream>
#include <string>
using namespace std;
class Student
{
private:
    string name;
    int age;
    char sex;
    string ID;
    float mathscore;
    float Chinesescore;

public:
    void setValue(string n, char s, int a, string id, float ms, float cs)
    {
        name = n;
        sex = s;
        age = a;
        ID = id;
        mathscore = ms;
        Chinesescore = cs;
    }
    void showValue()
    {
        cout << "name=" << name << endl;
        cout << "sex=" << sex << endl;
        cout << "age=" << age << endl;
        cout << "ID=" << ID << endl;
        cout << "mathscore=" << mathscore << endl;
        cout << "Chinesescore=" << Chinesescore << endl;
    }
    double calAveragescore()
    {
        return (mathscore + Chinesescore) / 2.0;
    }
};
int main()
{
    Student b;
    b.setValue("邓翔睿", 'F', 20, "B2021001", 85.1234f, 95.4321f);
    b.showValue();
    float averagescore = b.calAveragescore();
    cout << "averagescore=" << averagescore << endl;
    return 0;
}