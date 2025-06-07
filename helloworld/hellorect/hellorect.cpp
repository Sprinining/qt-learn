#include <iostream>
#include "rect.h"
using namespace std;

int main() {
    Rect arect(3.0, 4.0);
    cout << "Diagonal Length is " << arect.CalcDiagonal() << endl;
    cout << "Area is " << arect.CalcArea() << endl;
    return 0;
}