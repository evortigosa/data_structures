/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

/**
 * Classe que implementa uma Lista Duplamente Ligada de vértices de um grafo.
 * Essa classe é de manipulação interna do sistema e, portanto, não lança exceções.
 *
 * @author EVANDRO
 */
public class Vertice implements ListaDupla {
    private NoVertice inicio, fim;
    private int numVertices;

    /**
     * Construtor da classe Vertice.
     */
    public Vertice() {
        this.inicio= null;
        this.fim= null;
        this.numVertices= 0;
    }
    /**
     * Obtém o nó inicial da Lista de vértices.
     * @return Nó inicial da Lista
     */
    public NoVertice getInicio () {
        return this.inicio;
    }
    /**
     * Obtém o nó final da Lista de vértices.
     * @return Nó final da Lista
     */
    public NoVertice getFim () {
        return this.fim;
    }

    @Override
    public boolean listaVazia() {
        return (this.numVertices== 0);
    }

    @Override
    public int tamanhoLista() {
        return this.numVertices;
    }

    @Override
    public Object insereNaLista(Object objeto) {
        // Elemento inserido no vértice é uma String com o nome de um ator
        NoVertice auxiliar= new NoVertice(objeto);

        if (this.listaVazia()) {
            this.inicio= auxiliar;     // Caso em que o elemento é o primeiro
            this.fim= auxiliar;        // inserido na lista de vértices
        }
        else {
            this.fim.setProximo(auxiliar);  // Demais casos, nós são inseridos no
            auxiliar.setAnterior(this.fim); // final da lista de vértices
            this.fim= auxiliar;
        }
        this.numVertices++;       // Uma unidade é acrescida ao tamanho da Lista

        return auxiliar;
    }

    @Override
    public Object removeDaLista(Object objeto) {
        NoVertice auxiliar= ((NoVertice)objeto);
        Object ator= auxiliar.getAtor();

        if (this.numVertices== 1) {
            this.inicio= null;         // Caso em que o nó é o único inserido na
            this.fim= null;            // lista de vértices
        }
        else {          // Caso geral, remoção de nó qualquer em uma lista ligada dupla
            if (this.inicio== auxiliar) this.inicio= auxiliar.getProximo();

            if (this.fim== auxiliar) this.fim= auxiliar.getAnterior();

            if (auxiliar.getProximo()!= null) auxiliar.getProximo().setAnterior(auxiliar.getAnterior());

            if (auxiliar.getAnterior()!= null) auxiliar.getAnterior().setProximo(auxiliar.getProximo());
        }
        this.numVertices--;            // Uma unidade é decrescida do tamanho da Lista

        return ator;
    }

    @Override
    public Object existeElemento(Object objeto) {
        String nomeVertice, nomeBusca= ((String)objeto);
        NoVertice auxiliar= this.getFim();

        for (int i= 0; i< this.numVertices; i++) {         // Busca sequencial simples por um elemento possivelmente
            nomeVertice= ((String)auxiliar.getAtor());     // contido em um dos nós da lista de vértices

            if (nomeVertice.compareTo(nomeBusca)== 0) return auxiliar;
                                                           // Caso o usuário tivesse acesso a inserção e remoção de
            auxiliar= auxiliar.getAnterior();              // vértices, usaria o método compareToIgnoreCase()
        }
        return null;         // null é retornado em caso de elemento não encontrado
    }

    /**
     * Classe NoVertice que representa cada um dos nós de uma Lista duplamente
     * ligada de vértices do grafo.
     */
    protected class NoVertice {
        private Object ator;
        private int distancia;
        private NoVertice anterior;
        private NoVertice proximo;
        private NoVertice pai;
        private NoAdjacente listaAdjacencias;

        /**
         * Construtor da classe NoVertice.
         * @param ator Dados do ator que rotula o vértice
         */
        protected NoVertice(Object ator) {
            this.ator= ator;
            this.anterior= null;
            this.proximo= null;
            this.pai= null;
            this.listaAdjacencias= null;
        }
        /**
         * Obtém os dados do ator que rotula o vértice.
         * @return Dados do ator que rotula o vértice
         */
        public Object getAtor() {
            return this.ator;
        }
        /**
         * Configura a distância do vértice a algum outro, mais utilizada durante
         * o cálculo de caminhos mínimos.
         * @param distancia Inteiro representando o valor da distância
         */
        public void setDistancia(int distancia) {
            this.distancia= distancia;
        }
        /**
         * Obtém a distância do vértice a algum outro.
         * @return Inteiro representando o valor da distância
         */
        public int getDistancia() {
            return this.distancia;
        }
        /**
         * Configura a referência ao nó anterior da Lista de Vértices.
         * @param anterior Referência ao nó anterior
         */
        public void setAnterior(NoVertice anterior) {
            this.anterior= anterior;
        }
        /**
         * Obtém a referência ao nó anterior da Lista de Vértices.
         * @return Referência ao nó anterior
         */
        public NoVertice getAnterior() {
            return this.anterior;
        }
        /**
         * Configura a referência ao próximo nó da Lista de Vértices.
         * @param proximo Referência ao próximo nó
         */
        public void setProximo(NoVertice proximo) {
            this.proximo= proximo;
        }
        /**
         * Obtém a referência ao próximo nó da Lista de Vértices.
         * @return Referência ao próximo nó
         */
        public NoVertice getProximo() {
            return this.proximo;
        }
        /**
         * Configura a referência ao nó Pai do vértice atual, mais utilizada durante
         * a execução do algoritmo de Dijkstra.
         * @param pai Referência ao nó Pai do vértice atual
         */
        public void setPai(NoVertice pai) {
            this.pai= pai;
        }
        /**
         * Obtém a referência ao nó Pai do vértice atual.
         * @return Referência ao nó Pai do vértice atual
         */
        public NoVertice getPai() {
            return this.pai;
        }
        /**
         * Configura a referência ao primeiro elemento da Lista de Adjacências
         * de arestas incidentes do vértice.
         * @param adjacencia Referência ao primeiro elemento da Lista de Adjacências
         */
        public void setListaAdjacencias(NoAdjacente adjacencia) {
            this.listaAdjacencias= adjacencia;
        }
        /**
         * Obtém a referência ao primeiro elemento da Lista de Adjacências de
         * arestas incidentes do vértice.
         * @return Referência ao primeiro elemento da Lista de Adjacências
         */
        public NoAdjacente getListaAdjacencias() {
            return this.listaAdjacencias;
        }
    }
}
