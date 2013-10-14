#include <iostream>

using namespace std;

int main(void) {
    enum TEST {ONE, TWO, THREE};
    TEST t = ONE;
    if (t == ONE) {
        cout << "hello" << endl;
    }
    cout << ((TWO) << 2) << endl;
    return 0;
}
