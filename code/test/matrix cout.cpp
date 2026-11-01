#include <iostream>
using namespace std;
int main()
{
    int n, a, s;
    cin >> n;
    int array[n][n];
    for (int a = 0; a < n; a++)
    {
        for (int s = 0; s < n; s++)
        {
            cin >> array[a][s];
        }
    }
    for (a = 0; a < n; a++)
    {
        for (s = 0; s < n; s++)
        {
            if (a == s)
                cout << array[a][s] << " ";
        }
    }
    cout << endl;
    for (a = 0; a < n; a++)
    {
        for (s = 0; s < n; s++)
        {
            if (s + a == n - 1)
                cout << array[a][s] << " ";
        }
    }
    return 0;
}