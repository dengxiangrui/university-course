#include <iostream>
using namespace std;
int main()
{
    short n = 2;
    float a; // for循环语句还可以用double和float
    cin >> a;
    for (int m = 0; m < a; m++)
    {
        n *= 2;
    }
    cout << "2^" << a << " = " << n << endl;
    return 0;
}