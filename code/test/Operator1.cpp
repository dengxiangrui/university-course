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
    int y, l, w = 123456789;
    y = w >> 10, l = y ^ w;
    cout << w << "\t" << ~w << "\t" << l << endl;
    string a, s; // string是可以双定义的，与int等函数用法一致
    cin >> a >> s;
    if (a == "asdf" && s == "qwer")
        cout << "输入正确" << endl;
    else
        cout << "输入错误" << endl;
    double g, k;
    cin >> g >> k;
    if (g != 37 || k == 98)
        cout << "输入正确" << endl;
    else
        cout << "输入错误" << endl;
    cout << "程序运行中" << endl;
    system("pause");
    cout << "程序继续进行" << endl;
    bool is = !(1 <= 1 && 2 >= 9);
    cout << "asdf" << endl;
    cout << "zxcv" << "\n";
    cout << "qwer" << endl;
    cout << !is << endl;
    return 0;
}