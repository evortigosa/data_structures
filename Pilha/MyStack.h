/*        Aula Prática 4 - POO -
Evandro Scudeleti Ortigossa, nUSP 6793135 */

#ifndef MYSTACK_H_INCLUDED
#define MYSTACK_H_INCLUDED


class MyStack {
    public:
        MyStack(int);
        ~MyStack();
        unsigned int getSize() const;
        void print() const;
        void push(int elem, bool *error);
        int pop(bool *error);
        void invert();

    private:
        int maxsize;
        int top;
        int *stack;
};

#endif // MYSTACK_H_INCLUDED
