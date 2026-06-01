#ifndef COMPLX_H_INCLUDED
#define COMPLX_H_INCLUDED


class Complx {
    public:
        Complx(double= 0., double= 0.);
        Complx operator+(const Complx &) const;
        void print();

    private:
        double real;
        double imag;
};

#endif // COMPLX_H_INCLUDED
