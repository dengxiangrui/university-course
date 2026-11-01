#include <iostream>
using namespace std;
int main()
{
    int n, a, s, d, f;
    cin >> n;
    int array[n];
    if (n == 1)
    {
        cin >> array[0];
        cout << array[0];
    }
    else
    {
        for (int g = 0; g < n; g++)
        {
            cin >> array[g];
        }
        int min = array[0];
        for (int g = 1; g < n; g++)
        {
            if (min > array[g])
            {
                min = array[g];
                a = g;
            }
        }
        s = array[0];
        array[0] = array[a];
        array[a] = s;
        int max = array[0];
        for (int g = 1; g < n; g++)
        {
            if (max < array[g])
            {
                max = array[g];
                d = g;
            }
        }
        f = array[n - 1];
        array[n - 1] = array[d];
        array[d] = f;
        for (int g = 0; g < n; g++)
        {
            cout << array[g] << " ";
        }
    }
    return 0;
}