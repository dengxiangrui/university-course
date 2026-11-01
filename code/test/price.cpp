#include <iostream>
using namespace std;
int main()
{
    cout << "[1] apple" << endl;
    cout << "[2] pear" << endl;
    cout << "[3] orange" << endl;
    cout << "[4] grape" << endl;
    cout << "[0] exit" << endl;
    for (int i = 0; i < 5; i++)
    {
        int a;
        cin >> a;
        if (a == 1)
            cout << "price=3.00元/公斤" << endl;
        else if (a == 2)
            cout << "price=2.50元/公斤" << endl;
        else if (a == 3)
            cout << "price=4.10元/公斤" << endl;
        else if (a == 4)
            cout << "price=10.20元/公斤" << endl;
        else if (a == 0)
            break;
        else
            cout << "price=0" << endl;
    }
}