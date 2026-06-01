#include <iostream>
#include "Fila.h"

using namespace std;


int main() {
    Fila f1(9); //Pilha de no máximo 6 elementos
    Fila f2(12); //Pilha de no máximo 7 elementos
    bool error;
    int i, value;

    //Insere elementos na pilha f1
    i = 0;
    do {
        i += 1;
        value = i * 10;
        f1.enfileira(value, &error);
    } while (!error);

    //Exibe pilha f1
    cout << "Pilha f1 tem " << f1.getSize() << " elementos: ";
    f1.print();
    cout << endl;

    //Desenfileira elementos de f1
    value = f1.desenfileira(&error);
    while (!error) {
        cout << "Desempilhando " << value << endl;
        value = f1.desenfileira(&error);
    }

    //Insere elementos na Fila f2
    i = 0;
    do {
        i += 1;
        value = i * 10 + i;
        f2.enfileira(value, &error);
    } while (!error);

    //Exibe Fila f2
    cout << endl << "Fila f2 tem " << f2.getSize() << " elementos: ";
    f2.print();
    cout << endl;

    //Inverte Fila f2
    f2.invert();
    cout << "Fila f2 foi invertida" << endl;

    //Desenfileira elementos de f2
    value = f2.desenfileira(&error);
    while (!error) {
        cout << "Desempilhando " << value << endl;
        value = f2.desenfileira(&error);
    }

    return 0;

}
