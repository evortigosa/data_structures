/*        Aula Prática 4 - POO -
Evandro Scudeleti Ortigossa, nUSP 6793135 */

#include <iostream>
#include "MyStack.h"

using namespace std;

MyStack::MyStack(int maxsize) {
    this->maxsize= maxsize;
    top= -1;
    stack= new int[maxsize];
}

MyStack::~MyStack() {
    delete[] stack;
}

unsigned int MyStack::getSize() const {
    return (top+ 1);
}

void MyStack::print() const {
    int i;

    for (i= 0; i<= top; i++) cout << stack[i] << " ";
}

void MyStack::push(int elem, bool *error) {

    if (top>= (maxsize- 1)) (*error)= true;
    else {
        (*error)= false;

        top++;
        stack[top]= elem;
    }
}

int MyStack::pop(bool *error) {

    if (top< 0) (*error)= true;
    else {
        (*error)= false;

        top--;
        return stack[top +1];
    }
    return 0;
}

void MyStack::invert() {
    int auxiliar, i, tamanho= (top/ 2);

    for (i= 0; i< tamanho; i++) {
        auxiliar= stack[i];
        stack[i]= stack[top- i];
        stack[top- i]= auxiliar;
    }
}
