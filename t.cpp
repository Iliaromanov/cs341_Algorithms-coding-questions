#include <vector>
#include <iostream>

using namespace std;

int main() {
    vector<int> v(5, 0);
    for (auto i : v) {
        cout << i << endl;
    }
}