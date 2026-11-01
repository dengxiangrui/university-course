#include <iostream>
using namespace std;
int main()
{
    int n, minl, maxl;
    int x = -1, y = -1;
    cin >> n;
    int ma[n];
    int mi[n];
    int square[n][n];
    for (int s = 0; s < n; s++)
    {
        for (int d = 0; d < n; d++)
        {
            cin >> square[s][d];
        }
    }
    for (int s = 0; s < n; s++)
    {
        maxl = square[s][0];
        for (int d = 1; d < n; d++)
        {
            if (maxl < square[s][d])
            {
                maxl = square[s][d];
            }
        }
        ma[s] = maxl;
    }
    for (int d = 0; d < n; d++)
    {
        minl = square[0][d];
        for (int s = 1; s < n; s++)
        {
            if (minl > square[s][d])
            {
                minl = square[s][d];
            }
        }
        mi[d] = minl;
    }
    for (int s = 0; s < n; s++)
    {
        for (int d = 0; d < n; d++)
        {
            if (square[s][d] == mi[d] && square[s][d] == ma[s])
            {
                x = s;
                y = d;
            }
            else
                continue;
        }
    }
    if (x == -1 && y == -1)
        cout << "NONE" << endl;
    else
        cout << x << " " << y << endl;
    return 0;
}