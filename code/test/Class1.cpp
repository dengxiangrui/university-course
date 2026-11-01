#include <iostream>
#include <string>
using namespace std;
int main()
{
    double d, f;
    cin >> d >> f;
    if (d == 3 || f == 9) // 或运算符如果前一个条件成立，那么后一个条件就算有语法错误也不会继续执行
        cout << "输入正确" << endl;
    else
        cout << "输入错误" << endl;
    int g, h;
    g = 2, h = 9;
    (g += d) *= (f /= h);
    cout << g << endl;
    string c;
    int a;
    double b;
    cin >> c >> a >> b;
    cout << "“姓名：" << (string)c << "，年龄：" << (float)a << "，体重：" << (double)b << "kg”" << endl;
    return 0;
}