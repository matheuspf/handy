#include "ZipIter.h"
#include <bits/stdc++.h>

using namespace std;



int main ()
{
    vector<int> v = {0, 1, 2, 3, 4};
    vector<int> u = {5, 6, 7, 8, 9};

    const vector<int>& z = v;
    const vector<int>& w = u;

    cout << accumulate(ZIP_ALL(z, w), 0, it::unZip([](int sum, int a, int b){ return sum + a + b; })) << "\n";
    


    return 0;
}