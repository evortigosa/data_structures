/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import tadgrafo.Aresta.NoAresta;
import tadgrafo.Vertice.NoVertice;

/**
 * Classe que representa as Listas de Adjacências de um grafo.
 * A cada elemento da lista de vértices é adicionada uma referência para uma
 * lista de incidências (lista de referências para suas arestas incidentes) e
 * a cada elemento da lista de arestas são adicionadas referências para as
 * posições correspondentes nas listas de incidência dos seus vértices.
 *
 * @author EVANDRO
 */
public class NoAdjacente {
    private NoAresta aresta;
    private NoAdjacente anterior;
    private NoAdjacente proximo;

    /**
     * Construtor da classe NoAdjacente.
     * @param aresta Aresta referenciada pelo nó da estrutura de adjacências
     */
    public NoAdjacente(NoAresta aresta) {
        this.aresta= aresta;
        this.anterior= null;
        this.proximo= null;
    }
    /**
     * Obtém a aresta contida no grafo referenciada pelo nó da estrutura de
     * adjacências.
     * @return Aresta referenciada pelo nó da estrutura de adjacências
     */
    public NoAresta getAresta() {
        return this.aresta;
    }
    /**
     * Configura a referência ao nó anterior da Lista de Adjacências.
     * @param anterior Referência ao nó anterior
     */
    public void setAnterior(NoAdjacente anterior) {
        this.anterior= anterior;
    }
    /**
     * Obtém a referência ao nó anterior da Lista de Adjacências.
     * @return Referência ao nó anterior
     */
    public NoAdjacente getAnterior() {
        return this.anterior;
    }
    /**
     * Configura a referência ao próximo nó da Lista de Adjacências.
     * @param proximo Referência ao próximo nó
     */
    public void setProximo(NoAdjacente proximo) {
        this.proximo= proximo;
    }
    /**
     * Obtém a referência ao próximo nó da Lista de Adjacências.
     * @return Referência ao próximo nó
     */
    public NoAdjacente getProximo() {
        return this.proximo;
    }
    /**
     * Método responsável pela inserção de um nó na Lista de Adjacências de um
     * vértice.
     * @param vertice Vértice no qual o nó da Lista de Adjacências será inserido
     */
    public void insereAdjacencia(NoVertice vertice) {
        NoAdjacente auxiliar;
                                       // Vértice ainda não possui arestas incidentes
        if (vertice.getListaAdjacencias()== null) vertice.setListaAdjacencias(this);
        else {                         // Caso geral, vértice possui pelo menos uma aresta incidente
            auxiliar= vertice.getListaAdjacencias();
            auxiliar.setAnterior(this);
            this.setProximo(auxiliar);           // Em um método, a palavra-chave "this" faz referência
            vertice.setListaAdjacencias(this);   // ao objeto no qual o método opera
        }
    }
    /**
     * Método responsável pela remoção de um nó na Lista de Adjacências de um
     * vértice.
     * @param vertice Vértice no qual o nó da Lista de Adjacências será removido
     */
    public void removeAdjacencia(NoVertice vertice) {

        if ((this.proximo== null) && (this.anterior== null)) vertice.setListaAdjacencias(null);
        else if (this.proximo== null) this.anterior.setProximo(null);
        else if (this.anterior== null) {
            vertice.setListaAdjacencias(this.proximo);     // Todos os casos são considerados: se o vértice
            this.proximo.setAnterior(null);                // possui apenas uma aresta incidente, se ela é
        }                                                  // referenciada pelo primeiro ou último nó da lista
        else {                                             // de adjacências ou se está em um nó intermediário
            this.proximo.setAnterior(this.anterior);
            this.anterior.setProximo(this.proximo);
        }
    }
}
