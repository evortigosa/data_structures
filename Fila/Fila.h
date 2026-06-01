#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED


class Fila {
    public:
        Fila(int);
        ~Fila();
        void enfileira(int, bool *);
        int desenfileira(bool *);
        unsigned int getSize() const;
        void print() const;
        void invert();

    private:
        int maxSize;
        int top;
        int floor;
        int *queue;
};

#endif // FILA_H_INCLUDED
