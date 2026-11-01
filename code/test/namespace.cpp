#include <iostream>
using namespace std;
void fun()
{
    cout << "fun" << endl;
}
namespace bin
{
    int age = 18;
    void fun()
    {
        cout << "asdf" << endl;
    }
    namespace banned
    {
        void fun()
        {
            cout << "qwer" << endl;
        }
    }
}
int main()
{
    fun();
    bin::fun();
    bin::banned::fun();
    cout << "age=" << bin::age << endl;
    return 0;
}
