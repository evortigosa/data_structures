/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import tadgrafo.Vertice.NoVertice;

/**
 * Classe que representa uma Fila de Prioridades, ou seja, inserção e remoção
 * respeitam o conceito de Fila (FIFO), no entanto elementos de maior prioridade
 * (vértices do grafo com menor valor no campo distância) vão para o início da
 * Fila, de maneira ordenada, aonde são removidos.
 * Essa classe é de manipulação interna do sistema e, portanto, não lança exceções.
 *
 * @author EVANDRO
 */
public class FilaDePrioridades implements Fila {
    private NoFila inicio, fim;
    private int tamanhoFila;

    /**
     * Construtor da classe FilaDePrioridades.
     */
    public FilaDePrioridades() {
        this.inicio= null;
        this.fim= null;
        this.tamanhoFila= 0;
    }
    /**
     * Obtém o nó de início da Fila.
     * @return Nó inicial da Fila
     */
    public NoFila getInicio() {
        return this.inicio;
    }
    /**
     * Obtém o nó do final da Fila.
     * @return Nó final da Fila
     */
    public NoFila getFim() {
        return this.fim;
    }

    @Override
    public boolean filaVazia() {
        return (this.tamanhoFila== 0);
    }

    @Override
    public int tamanhoFila() {
        return this.tamanhoFila;
    }

    @Override
    public void enfileira(Object objeto) {
        // Elemento inserido na Fila é a referência a um vértice do grafo
        NoVertice vertice= ((NoVertice)objeto);
        NoFila auxiliar= new NoFila(vertice);

        if (filaVazia()) {
            this.inicio= auxiliar;     // Nó é o primeiro inserido na Fila
            this.fim= auxiliar;
        }
        else {
            if (vertice.getDistancia()== 0) {         // Caso a prioridade (campo distancia do vértice)
                this.inicio.setAnterior(auxiliar);    // for máxima (zero), nó já é inserido no topo da
                auxiliar.setProximo(this.inicio);     // Fila, caso contrário, é inserido no final
                this.inicio= auxiliar;
            }
            else {
                this.fim.setProximo(auxiliar);        // Inserção tradicional de nó no final da Fila
                auxiliar.setAnterior(this.fim);
                this.fim= auxiliar;
            }
        }
        this.tamanhoFila++;       // Uma unidade é acrescida ao tamanho da Fila
    }

    @Override
    public Object desenfileira() {
        NoVertice vertice= this.inicio.getItem();

        if (this.tamanhoFila== 1) {
            this.inicio= null;         // Nó é o único
            this.fim= null;
        }
        else {                         // Caso geral, remoção do topo da Fila
            this.inicio= this.inicio.getProximo();
            this.inicio.setAnterior(null);
        }
        this.tamanhoFila--;            // Uma unidade é decrescida do tamanho da Fila

        return vertice;
    }
    /**
     * Método responsável pela reorganização de uma Fila de Prioridades, em que,
     * os elementos de maior prioridade são organizados no topo da Fila.
     * @param objeto Item da Fila que teve sua prioridade modificada e deve ser
     * reorganizado
     */
    public void reorganizaFila(Object objeto) {
        NoFila auxFila= this.getFim();
        NoVertice vertice= ((NoVertice)objeto);

        while((auxFila.getAnterior()!= null) && ((auxFila.getItem())!= vertice)) auxFila= auxFila.getAnterior();

        while((auxFila.getAnterior()!= null) && ((auxFila.getItem().getDistancia())< (auxFila.getAnterior().getItem().getDistancia()))) {
            vertice= auxFila.getItem();
            auxFila.setItem(auxFila.getAnterior().getItem());
            auxFila.getAnterior().setItem(vertice);   // Envio de elemento de maior prioridade a uma
                                                      // posição conveniente mais ao topo da Fila
            auxFila= auxFila.getAnterior();
        }
    }

    /**
     * Classe NoFila que representa cada um dos nós de uma Fila.
     */
    protected class NoFila {
        private NoVertice vertice;
        private NoFila anterior;
        private NoFila proximo;

        /**
         * Construtor da classe NoFila.
         * @param vertice Referência a um vértice do grafo
         */
        protected NoFila(NoVertice vertice) {
            this.vertice= vertice;
            this.anterior= null;
            this.proximo= null;
        }
        /**
         * Configura o item (referência a um vértice) contido no nó da Fila.
         * @param vertice Referência a um vértice do grafo
         */
        public void setItem(NoVertice vertice) {
            this.vertice= vertice;
        }
        /**
         * Obtém o item (referência a um vértice) contido no nó da Fila.
         * @return Referência a um vértice do grafo
         */
        public NoVertice getItem() {
            return this.vertice;
        }
        /**
         * Configura a referência ao nó anterior da Fila.
         * @param anterior Referência ao nó anterior
         */
        public void setAnterior(NoFila anterior) {
            this.anterior= anterior;
        }
        /**
         * Obtém a referência ao nó anterior da Fila.
         * @return Referência ao nó anterior
         */
        public NoFila getAnterior() {
            return this.anterior;
        }
        /**
         * Configura a referência ao próximo nó da Fila.
         * @param proximo Referência ao próximo nó
         */
        public void setProximo(NoFila proximo) {
            this.proximo= proximo;
        }
        /**
         * Obtém a referência ao próximo nó da Fila.
         * @return Referência ao próximo nó
         */
        public NoFila getProximo() {
            return this.proximo;
        }
    }
}
