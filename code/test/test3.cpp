#include <iostream>
#include <string> //string其实算一个字符数组
using namespace std;
int main()
{
    string num; // 由于num的数字量较大，假如比long long int的MAX值还大，就只能用string(理论上可以弄成无穷大)
    cin >> num;
    for (int i = 0; i < num.length(); i++)
    {
        cout << num[i];            // i只能从0开始，因为num[i]是从0开始的
        if (i != num.length() - 1) // 这里的num[i]实际上只是字符，而不是数字，换算成数字要用num[i]-'0'
            cout << "   ";
    }
    return 0;
}