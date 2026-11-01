#include <iostream>
#include <string>
using namespace std;
int main()
{
    int a;
    string g;
    cin >> a >> g;
    int b = a * 2;
    for (int s = 1; s <= b; s++)
    {
        if (s % 2 == 0)
            cout << endl;
        else
        {
            for (int d = 0; d < (b - s) / 2; d++)
            {
                cout << " ";
            }
            for (int f = 0; f < s; f++)
            {
                cout << g;
            }
            cout << endl;
        }
    }
    return 0;
}