#include <iostream>
using namespace std;
int main()
{
    long long int a = 2, w = 7.345667788909855433; // 实际上w=7
    float b = 3.67f;
    double c = 7898651.567677876543;
    double k = b + c;
    string s("asdfghjkl");
    bool d = 123456789;
    cout << a << "\nyyy" << "\ttjjj" << b << "\nuuu" << c << "\nkloi" << s << "\n...." << k << "\t" << d << "\t" << w << "\n";
    cout << a + b * c << "\t" << (a + b) * c << "\t" << c + a / b << "\t" << w % a - b << endl;
    char x;
    cout << "请输入一个字符: ";
    cin >> x; // 使用 C++ 的输入方式
    if (x == 'a' || x == 'b')
        cout << "你输入的是 a 或 b" << endl;
    else
        cout << "你输入的不是 a 或 b" << endl;
    cout << "asdfghjkl" << endl;
    return 0;
}