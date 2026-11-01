#include <iostream>
using namespace std;
void sdf();
double add(double a, double d)
{
    return a + d; // int,short,double,float等前缀要输出值
    sdf();
}
void sdf()
{
    cout << 1.23456 << endl; // void前缀不输出值
}
int main()
{
    double b = add(89, 56);
    cout << (int)b << "\n";
    sdf();
    sdf();
}