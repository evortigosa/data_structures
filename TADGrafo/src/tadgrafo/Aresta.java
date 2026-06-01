/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import tadgrafo.Vertice.NoVertice;

/**
 * Classe que implementa uma Lista Duplamente Ligada de arestas de um grafo.
 * Essa classe é de manipulação interna do sistema e, portanto, não lança exceções
 *
 * @author EVANDRO
 */
public class Aresta implements ListaDupla {
    private NoAresta inicio, fim;
    private int numArestas;

    /**
     * Construtor da classe Aresta.
     */
    public Aresta() {
        this.inicio= null;
        this.fim= null;
        this.numArestas= 0;
    }
    /**
     * Obtém o nó inicial da Lista de arestas.
     * @return Nó inicial da Lista
     */
    public NoAresta getInicio () {
        return this.inicio;
    }
    /**
     * Obtém o nó final da Lista de arestas.
     * @return Nó final da Lista
     */
    public NoAresta getFim () {
        return this.fim;
    }

    @Override
    public boolean listaVazia() {
        return (this.numArestas== 0);
    }

    @Override
    public int tamanhoLista() {
        return this.numArestas;
    }

    @Override
    public Object insereNaLista(Object objeto) {
        // Elemento inserido na aresta é uma String com o nome de um filme
        NoAresta auxiliar= new NoAresta(objeto);

        if (this.listaVazia()) {
            this.inicio= auxiliar;     // Caso em que o elemento é o primeiro
            this.fim= auxiliar;        // inserido na lista de arestas
        }
        else {
            this.fim.setProximo(auxiliar);  // Demais casos, nós são inseridos no
            auxiliar.setAnterior(this.fim); // final da lista de arestas
            this.fim= auxiliar;
        }
        this.numArestas++;        // Uma unidade é acrescida ao tamanho da Lista

        return auxiliar;
    }

    @Override
    public Object removeDaLista(Object objeto) {
        NoAresta auxiliar= ((NoAresta)objeto);
        Object filme= auxiliar.getFilme();

        if (this.numArestas== 1) {
            this.inicio= null;         // Caso em que o nó é o único inserido na
            this.fim= null;            // lista de arestas
        }
        else {          // Caso geral, remoção de nó qualquer em uma lista ligada dupla
            if (this.inicio== auxiliar) this.inicio= auxiliar.getProximo();

            if (this.fim== auxiliar) this.fim= auxiliar.getAnterior();

            if (auxiliar.getProximo()!= null) auxiliar.getProximo().setAnterior(auxiliar.getAnterior());

            if (auxiliar.getAnterior()!= null) auxiliar.getAnterior().setProximo(auxiliar.getProximo());
        }
        this.numArestas--;             // Uma unidade é decrescida do tamanho da Lista

        return filme;
    }

    @Override
    public Object existeElemento(Object objeto) {
        String filmeAresta, filmeBusca= ((String)objeto);
        NoAresta auxiliar= this.getFim();

        for (int i= 0; i< this.numArestas; i++) {          // Busca sequencial simples por um elemento possivelmente
            filmeAresta= ((String)auxiliar.getFilme());    // contido em um dos nós da lista de arestas

            if (filmeAresta.compareTo(filmeBusca)== 0) return auxiliar;
                                                           // Caso o usuário tivesse acesso a busca de arestas, usaria
            auxiliar= auxiliar.getAnterior();              // o método compareToIgnoreCase()
        }
        return null;         // null é retornado em caso de elemento não encontrado
    }

    /**
     * Classe NoAresta que representa cada um dos nós de uma Lista duplamente
     * ligada de arestas do grafo.
     */
    protected class NoAresta {
        private Object filme;
        private int peso;
        private NoAresta anterior;
        private NoAresta proximo;
        private NoVertice verticeA;
        private NoVertice verticeB;
        private NoAdjacente adjVerticeA;
        private NoAdjacente adjVerticeB;

        /**
         * Construtor da classe NoAresta.
         * @param filme Dados do filme que rotula a aresta
         */
        protected NoAresta(Object filme) {
            this.filme= filme;
            this.peso= 1;
            this.anterior= null;
            this.proximo= null;
            this.verticeA= null;
            this.verticeB= null;
            this.adjVerticeA= null;
            this.adjVerticeB= null;
        }
        /**
         * Obtém os dados do filme que rotula a aresta.
         * @return Dados do filme que rotula a aresta
         */
        public Object getFilme() {
            return this.filme;
        }
        /**
         * Obtém o peso da aresta. Para esta implementação, o campo peso foi
         * fixado como unitário.
         * @return Inteiro representando o peso da aresta
         */
        public int getPeso() {
            return this.peso;
        }
        /**
         * Configura a referência ao nó anterior da Lista de Arestas.
         * @param anterior Referência ao nó anterior
         */
        public void setAnterior(NoAresta anterior) {
            this.anterior= anterior;
        }
        /**
         * Obtém a referência ao nó anterior da Lista de Arestas.
         * @return Referência ao nó anterior
         */
        public NoAresta getAnterior() {
            return this.anterior;
        }
        /**
         * Configura a referência ao próximo nó da Lista de Arestas.
         * @param proximo Referência ao próximo nó
         */
        public void setProximo(NoAresta proximo) {
            this.proximo= proximo;
        }
        /**
         * Obtém a referência ao próximo nó da Lista de Arestas.
         * @return Referência ao próximo nó
         */
        public NoAresta getProximo() {
            return this.proximo;
        }
        /**
         * Configura a referência a um dos vértices incidentes da aresta.
         * @param verticeA Referência a um vértice incidente
         */
        public void setVerticeA(NoVertice verticeA) {
            this.verticeA= verticeA;
        }
        /**
         * Obtém a referência a um dos vértices incidentes da aresta.
         * @return Referência a um vértice incidente
         */
        public NoVertice getVerticeA() {
            return this.verticeA;
        }
        /**
         * Configura a referência a um dos vértices incidentes da aresta.
         * @param verticeB Referência a um vértice incidente
         */
        public void setVerticeB(NoVertice verticeB) {
            this.verticeB= verticeB;
        }
        /**
         * Obtém a referência a um dos vértices incidentes da aresta.
         * @return Referência a um vértice incidente
         */
        public NoVertice getVerticeB() {
            return this.verticeB;
        }
        /**
         * Configura a referência a uma das estruturas de adjacências de um dos
         * vértices incidentes da aresta.
         * @param adjVA Estrutura de adjacências que referencia a aresta
         */
        public void setAdjVerticeA(NoAdjacente adjVA) {
            this.adjVerticeA= adjVA;
        }
        /**
         * Obtém a referência a uma das estruturas de adjacências de um dos
         * vértices incidentes da aresta.
         * @return Estrutura de adjacências que referencia a aresta
         */
        public NoAdjacente getAdjVerticeA() {
            return this.adjVerticeA;
        }
        /**
         * Configura a referência a uma das estruturas de adjacências de um dos
         * vértices incidentes da aresta.
         * @param adjVB Estrutura de adjacências que referencia a aresta
         */
        public void setAdjVerticeB(NoAdjacente adjVB) {
            this.adjVerticeB= adjVB;
        }
        /**
         * Obtém a referência a uma das estruturas de adjacências de um dos
         * vértices incidentes da aresta.
         * @return Estrutura de adjacências que referencia a aresta
         */
        public NoAdjacente getAdjVerticeB() {
            return this.adjVerticeB;
        }
    }
}
