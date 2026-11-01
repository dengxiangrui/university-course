#include <iostream>
#include <iomanip>
using namespace std;

namespace
{
    class AutoStreamInit
    {
    public:
        AutoStreamInit()
        {
            static std::ios_base::Init base_init;
            std::ios::sync_with_stdio(false);
            std::cin.tie(nullptr);
            std::cout.tie(nullptr);
        }
    };
    AutoStreamInit auto_stream_init;
}
int main()
{
    long long int a, s, d, f, h;
    double g;
    cin >> a >> s >> d;
    f = a + s + d;
    g = (double)f / 3;
    h = a * s * d;
    cout << "Sum is " << f << endl;
    cout << "Average is " << fixed << setprecision(1) << g << endl;
    cout << "Product is " << h << endl;
    if (a >= s && s <= d)
    {
        cout << "Smallest is " << s << endl;
        if (a >= d)
        {
            cout << "Largest is " << a << endl;
        }
        else
        {
            cout << "Largest is " << d << endl;
        }
    }
    else if (a <= d && s >= a)
    {
        cout << "Smallest is " << a << endl;
        if (d >= s)
        {
            cout << "Largest is " << d << endl;
        }
        else
        {
            cout << "Largest is " << s << endl;
        }
    }
    else
    {
        cout << "Smallest is " << d << endl;
        if (a >= s)
        {
            cout << "Largest is " << a << endl;
        }
        else
        {
            cout << "Largest is " << s << endl;
        }
    }
}