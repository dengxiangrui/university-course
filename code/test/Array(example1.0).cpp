#include <iostream>
using namespace std;
int main()
{
    int s;
    cin >> s;
    int a[s];
    for (int d = 0; d < s; d++)
    {
        cin >> a[d];
    }
    int num = a[0];
    for (int f = 1; f < s; f++)
    {
        if (num < a[f])
        {
            num = a[f];
        }
        else
            continue;
    }
    cout << num << " ";
    for (int g = 0; g < s; g++)
    {
        if (a[g] == num)
        {
            cout << g << endl;
            break;
        }
        else
            continue;
    }
    return 0;
}
