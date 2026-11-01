#include <iostream>
#include <string>
using namespace std;
int main()
{
    int array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int num[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    string a;
    cin >> a;
    for (int s = 0; s < a.length(); s++)
    {
        for (int d = 0; d < 10; d++)
        {
            if (array[d] == a[s])
            {
                num[d] += 1;
            }
            else
                continue;
        }
    }
    for (int s = 0; s < a.length(); s++)
    {
        if (num[s] != 0)
            cout << array[s] << " " << num[s] << endl;
        else
            continue;
    }
    return 0;
}