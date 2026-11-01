#include <iostream>
using namespace std;
int main()
{
    double score, number;
    cout << "please input number of people" << "\n";
    cin >> number;
    for (int i = 0; i < number; i++)
    {
        cout << "please input score:";
        cin >> score;
        if (!cin.good()) // 判断数据是否符合定义类型
        {
            cerr << "error:It is not type of double" << endl; // 输出错误信息
            cin.clear();
            cin.sync(); // 清空输入流
            i--;
        }
        else
        {
            if (score < 0 || score > 100)
            {
                cerr << "error:It is not between 0 and 100";
                i--;
            }
            else if (score >= 80)
                cout << 'A';
            else if (score >= 70)
                cout << 'B';
            else if (score >= 60)
                cout << 'C';
            else
                cout << 'D';
        }
    }
    return 0;
}