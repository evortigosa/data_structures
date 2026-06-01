/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import java.io.Serializable;

/**
 * Interface para uma Lista duplamente ligada, ou seja, cada nó da lista mantém
 * uma referência para o nó anterior e posterior.
 *
 * @author EVANDRO
 */
public interface ListaDupla extends Serializable {

    /**
     * Método informa se a Lista está vazia ou não.
     * @return Verdadeiro se a Lista estiver vazia, falso em caso contrário
     */
    public boolean listaVazia();
    /**
     * Método informa o número de elementos na Lista.
     * @return Inteiro representando o tamanho da Lista
     */
    public int tamanhoLista();
    /**
     * Método responsável pela inserção de um nó em uma Lista duplamente ligada.
     * @param objeto Elemento contido no nó que será inserido na Lista
     * @return Referência ao nó inserido na Lista
     */
    public Object insereNaLista(Object objeto);
    /**
     * Método responsável pela remoção de um nó qualquer de uma Lista duplamente
     * ligada.
     * @param objeto Referência ao nó da lista que será removido
     * @return Elemento contido dentro do nó removido
     */
    public Object removeDaLista(Object objeto);
    /**
     * Método responsável pela busca de um nó contendo o objeto especificado.
     * @param objeto Elemento contido dentro de um nó da Lista
     * @return Referência nó que contém o elemento buscado, caso exista, ou null
     * caso não exista
     */
    public Object existeElemento(Object objeto);

}
