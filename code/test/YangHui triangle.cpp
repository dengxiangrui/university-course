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
    int n;
    cin >> n;                   // 创建二维数组存储杨辉三角
    int triangle[10][10] = {0}; // 生成杨辉三角
    for (int i = 0; i < n; i++)
    {
        triangle[i][0] = 1; // 每行第一个元素为1
        triangle[i][i] = 1; // 每行最后一个元素为1
        for (int j = 1; j < i; j++)
        {
            triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
        }
    } // 打印杨辉三角
    for (int i = 0; i < n; i++)
    { // 打印每行前面的空格，形成正三角形
        for (int k = 0; k < n - i - 1; k++)
        {
            cout << " ";
        } // 打印每行的数字
        for (int j = 0; j <= i; j++)
        {
            cout << setw(4) << triangle[i][j];
        }
        cout << endl;
    }
    return 0;
}