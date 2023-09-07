// C++ program to demonstrate
// array of strings using
// string class
#include <bits/stdc++.h>
#include <string>
using namespace std;

// Driver code
int main()
{
    // Initialize String Array
    std::string colour[4];
    colour[1]="abc";

    // Print Strings
    for (int i = 0; i < 4; i++)
        std::cout << colour[i] << "\n";
}
