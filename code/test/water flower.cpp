#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    int N;
    cin >> N; // 计算N位数的范围
    int start = pow(10, N - 1);
    int end = pow(10, N) - 1; // 遍历所有N位数
    for (int num = start; num <= end; num++)
    {
        int sum = 0;
        int temp = num; // 计算每位数字的N次幂之和
        while (temp > 0)
        {
            int digit = temp % 10;
            sum += pow(digit, N);
            temp /= 10;
        } // 判断是否为水仙花数
        if (sum == num)
        {
            cout << num << endl;
        }
    }
    return 0;
}