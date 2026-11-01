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
    int a1, b1, a2, b2;
    char slash; // 用于读取斜杠
    // 读取第一个分数 a1/b1
    cin >> a1 >> slash >> b1;
    // 读取第二个分数 a2/b2
    cin >> a2 >> slash >> b2;
    // 比较两个分数的大小（交叉相乘）
    if (a1 * b2 < a2 * b1)
        cout << a1 << "/" << b1 << " < " << a2 << "/" << b2;
    else if (a1 * b2 == a2 * b1)
        cout << a1 << "/" << b1 << " = " << a2 << "/" << b2;
    else
        cout << a1 << "/" << b1 << " > " << a2 << "/" << b2;
    system("pause");
    return 0;
}