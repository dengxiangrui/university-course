#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    double a, b, c, x, p, y, q;
    cin >> a >> b >> c;
    p = (-b) / (2 * a);
    q = sqrt(b * b - 4 * a * c) / (2 * a); // 这里的sqrt只计算了前面的小括号，没有计算后面的小括号
    if (a == 0)
    {
        cout << "不是" << endl;
    }
    else
    {
        if (b * b - 4 * a * c == 0)
        {
            x = p;
            y = p;
            cout << x << "\t" << y << endl;
        }
        else if (b * b - 4 * a * c > 0)
        {
            x = p + q;
            y = p - q;
            cout << x << "\t" << y << endl;
        }
        else
        {
            cout << "不是" << endl;
        }
    }
    return 0;
}