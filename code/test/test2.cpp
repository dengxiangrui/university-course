// ÆåÅÌÎÊÌâ
#include <iostream>
#include <string>
using namespace std;
int main()
{
    string s;
    double N;
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        if (i % 2 == 0)
            s = "* ";
        else
            s = " *";
        for (int j = 0; j < N; j++)
        {
            cout << s;
        }
        cout << endl;
    }
    return 0;
}