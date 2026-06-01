#include <iostream>
#include "Complx.h"

using namespace std;


Complx::Complx(double real, double imag) {
    this->real= real;
    this->imag= imag;
}

Complx Complx::operator+(const Complx &c) const {
    Complx resultado;

    resultado.real= (this->real + c.real);
    resultado.imag= (this->imag + c.imag);

    return resultado;
}

void Complx::print() {
    cout << real << " + " << imag << "*i" << endl;
}
