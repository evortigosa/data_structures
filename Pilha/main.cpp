/*        Aula Prática 4 - POO -
Evandro Scudeleti Ortigossa, nUSP 6793135 */

#include <iostream>
#include "MyStack.h"

using namespace std;

int main() {
    MyStack s1(6); //Pilha de no máximo 6 elementos
    MyStack s2(7); //Pilha de no máximo 7 elementos
    bool error;
    int i, value;

    //Insere elementos na pilha s1
    i = 0;
    do {
        i += 1;
        value = i * 10;
        s1.push(value, &error);
    } while (!error);

    //Exibe pilha s1
    cout << "Pilha s1 tem " << s1.getSize() << " elementos: ";
    s1.print();
    cout << endl;

    //Desempilha elementos de s1
    value = s1.pop(&error);
    while (!error) {
        cout << "Desempilhando " << value << endl;
        value = s1.pop(&error);
    }

    //Insere elementos na pilha s2
    i = 0;
    do {
        i += 1;
        value = i * 10 + i;
        s2.push(value, &error);
    } while (!error);

    //Exibe pilha s2
    cout << endl << "Pilha s2 tem " << s2.getSize() << " elementos: ";
    s2.print();
    cout << endl;

    //Inverte pilha s2
    s2.invert();
    cout << "Pilha s2 foi invertida" << endl;

    //Desempilha elementos de s2
    value = s2.pop(&error);
    while (!error) {
        cout << "Desempilhando " << value << endl;
        value = s2.pop(&error);
    }

return 0;

}
