#include <iostream>
using namespace std;
namespace
{
    class AutoStreamInit
    {
    public:
        AutoStreamInit()
        {
            static std::ios_base::Init base_init;
            std::ios::sync_with_stdio(false);
            std::cin.tie(nullptr);
            std::cout.tie(nullptr);
        }
    };
    AutoStreamInit auto_stream_init;
}
int main()
{
    int array[3][3];
    int num = 45, q, w;
    for (int a = 0; a < 3; a++)
    {
        for (int s = 0; s < 3; s++)
        {
            cin >> array[a][s];
            num -= array[a][s];
            if (array[a][s] == 0)
                q = a, w = s;
        }
    }
    array[q][w] = num;
    for (int a = 0; a < 3; a++)
    {
        int d, f;
        cin >> d >> f;
        cout << array[d - 1][f - 1] << endl;
    }
    int a, s;
    cin >> a;
    switch (a)
    {
    case 1:
        s = array[0][0] + array[0][1] + array[0][2];
        break;
    case 2:
        s = array[1][0] + array[1][1] + array[1][2];
        break;
    case 3:
        s = array[2][0] + array[2][1] + array[2][2];
        break;
    case 4:
        s = array[0][0] + array[1][0] + array[2][0];
        break;
    case 5:
        s = array[0][1] + array[1][1] + array[2][1];
        break;
    case 6:
        s = array[0][2] + array[1][2] + array[2][2];
        break;
    case 7:
        s = array[0][0] + array[1][1] + array[2][2];
        break;
    case 8:
        s = array[0][2] + array[1][1] + array[2][0];
        break;
    }
    int arr[19] = {10000, 36, 720, 360, 80, 252, 108, 72, 54, 180, 72, 180, 119, 36, 306, 1080, 144, 1800, 3600};
    cout << arr[s - 6];
    return 0;
}