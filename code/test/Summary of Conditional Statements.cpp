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
    double n;
    cin >> n;
    if (n > 0 && n <= 20)
    {
        cout << "血量偏低" << endl;
    }
    else if (n > 20 && n <= 80)
    {
        cout << "血量正常" << endl;
    }
    else if (n > 80)
    {
        cout << "血量高" << endl;
    }
    else
    {
        cout << "已死亡" << endl;
    }
    long long a; // 注意此处输入方式不能是浮点型，switch内不能是小数
    cin >> a;
    switch (a)
    {
    case 1:
        cout << 1 << endl;
        /*{
            cout << 1 << endl;
            cout << 1 << endl;
            cout << 1 << endl;
            cout << 1 << endl;
        }
            代码太多可以加一个大括号，从而不影响输出*/
        break; // 一定要有break，不然假如输入1，此时case 1和case 2都会执行，结果就是1和2
    case 2:
        cout << 2 << endl;
        break;
    case 3:
    case 4:
        cout << 3 << endl; // 此时3和4都是一种情况，都是输出3.即case语句可以叠加
        break;
    default:
        cout << "无匹配值" << endl;
    }
    for (double m = 0;; m += 10)
    {
        if (m <= 500)
            cout << m << endl;
        else
            break; // break的作用：结束循环语句的运行并跳出本次循环语句
    } // 注意第二个代码是限定条件，用来避免成为死循环
    short i = 0;
    while (i < 100)
    {
        i++;
        if (i % 2 == 0)
            cout << i << endl;
        else
            continue; // continue的作用：跳过本次循环语句（只是不输出本次结果，并不是跳出程序）
        // 注：假如在continue的后面还有一个i+=10.那么如果执行if,则i+=10也会执行；如果执行else,则i+=10不会执行
    }
name:
    cout << 1234567890 << endl; // name是一个定义的事件

    double g = 1;
    do
    {
        g += 2;
        if (g <= 10 && g >= 20)
        {
            cout << 987654321 << endl;
        }
        else
        {
            goto name; // goto只能返回到上面的
        }
    } while (g < 100); // do-while循环至少会循环一次，因为是先do(执行)，再判断正误
    return 0;
}