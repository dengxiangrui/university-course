#include <iostream>
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
    int a, b;
    string s;
    cin >> a >> s;
    for (int d = 0; d < a; d++)
    {
        if (d % 2 == 0)
        {
            for (int g = 0; g < a; g++)
            {
                cout << s;
            }
            cout << "\n";
        }
        else
            cout << "\n";
    }
    system("pause");
}