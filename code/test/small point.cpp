#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    const double a = 3.1415926535, b = 9.087654321;
    cout << fixed << setprecision(2) << a << endl;
    cout << fixed << setprecision(4) << a << endl;
    cout << fixed << setprecision(6) << a << endl;
    cout << setprecision(2) << b << endl;
    cout << setprecision(4) << b << endl;
    cout << setprecision(6) << b << endl;
    // 有fixed表示小数点后保留几位小数，无fixed表示有效数字是几位
    return 0;
}