/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import java.io.Serializable;

/**
 * Interface para uma Fila, ou seja, estrutura linear na qual as inserções são
 * feitas em um extremo (final) e remoções são feitas no outro extremo (início).
 *
 * @author EVANDRO
 */
public interface Fila extends Serializable {

    /**
     * Método informa se a Fila está vazia.
     * @return Verdadeiro se a Fila estiver vazia, falso em caso contrário
     */
    public boolean filaVazia();
    /**
     * Método informa o número de elementos na Fila.
     * @return Um inteiro representando o número de elementos na Fila
     */
    public int tamanhoFila();
    /**
     * Método responsável pela inserção de um elemento no final da Fila.
     * @param objeto Elemento contido no novo nó inserido na Fila
     */
    public void enfileira(Object objeto);
    /**
     * Método responsável pela remoção do elemento a frente da Fila.
     * @return Item contido no nó removido
     */
    public Object desenfileira();

}