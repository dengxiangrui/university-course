#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    long long int n, o = 1, a = 10;
    double s = 1;
    cin >> n;
    for (int d = 0; d < n; d++)
    {
        a *= 10;
    }
    for (int i =1 ;; i++)
    {
        o *= i;
        if (o <= a)
            s += static_cast<double>(1) / o;
        else
            break;
    }
    cout << fixed << setprecision(n + 1) << s << endl;
    return 0;
}