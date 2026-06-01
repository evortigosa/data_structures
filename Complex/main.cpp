#include <iostream>
#include "Complx.h"

using namespace std;


int main() {
    Complx x(3, 4);
    Complx y(6, 7);

    Complx z= x + y;
    z.print();

    return 0;

}
