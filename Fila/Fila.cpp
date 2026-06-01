#include "Fila.h"
#include <iostream>

using namespace std;


Fila::Fila(int maxSize) {
    if (maxSize< 10) maxSize= 10;

    this->maxSize= maxSize;
    this->top= 0;
    this->floor= 0;
    this->queue= new int[maxSize];
}

Fila::~Fila() {
    delete[] this->queue;
}

void Fila::enfileira(int elem, bool *error) {

    if (floor>= maxSize) (*error)= true;
    else {
        (*error)= false;

        queue[floor]= elem;
        floor++;
    }
}

int Fila::desenfileira(bool *error) {
    int elem;

    if (top>= floor) (*error)= true;
    else {
        (*error)= false;

        elem= queue[top];
        top++;

        return elem;
    }
}

unsigned int Fila::getSize() const {
    return (floor- top);
}

void Fila::print() const {
    int i;

    for (i= top; i< floor; i++) cout << queue[i] << " ";

    cout << endl;
}

void Fila::invert() {
    int auxiliar, i, tam= getSize()/ 2;

    for (i= top; i< tam; (i++))  {
        auxiliar= queue[i];
        queue[i]= queue[floor- (i+ 1)];
        queue[floor- (i+ 1)]= auxiliar;
    }
}
