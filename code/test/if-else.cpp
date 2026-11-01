#include <iostream>
using namespace std;
int main()
{
    long long int a, b;
    cin >> a >> b;
    cout << a << "+";
    if (b < 0)
        cout << "(" << b << ")";
    else
        cout << b;
    // else if (cout << b);
    cout << "=" << a + b << endl;
    cout << a << "*";
    if (b < 0)
        cout << "(" << b << ")";
    else
        cout << b;
    // else if (cout << b);
    cout << "=" << a * b << endl;
    cout << "程序运行中。。。";
    system("pause");
    cout << "程序继续运行。。。" << endl;
    if (b == 0)
    {
        cout << a << "/0=underfined" << "\n";
        cout << a << "%0=underfined" << "\n";
    }
    else
    {
        cout << a << "/";
        if (b < 0)
            cout << "(" << b << ")";
        else
            cout << b;
        // else if (cout << b);
        cout << "=" << a / b << endl;
        cout << a << "%";
        if (b < 0)
            cout << "(" << b << ")";
        else
            cout << b;
        // else if (cout << b);
        cout << "=" << a % b << endl;
    }
    return 0;
}