/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package graphickb;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.SortedSet;
import java.util.TreeSet;

/**
 * Classe responsável por pesquisar substrings em um ArrayList de strings,
 * organizando os resultados de maneira lexicograficamente ordenada e
 * inserindo-os em um componente ListaNomes.
 *
 * @author EVANDRO
 */
public final class Pesquisa implements Runnable {
    private ArrayList<String> nomes;
    private ListaNomes lista;
    private String subString;
    private SortedSet<String> pesquisa;     // SortedSet que receberá o resultado final da pesquisa

    /**
     * Construtor da classe Pesquisa.
     * @param lista ListaNomes em que o resultado da pesquisa será inserido
     * @param nomes ArrayList de strings que servirá de base para a pesquisa
     * @param subString Substring que será buscada dentro do ArrayList
     */
    public Pesquisa(ListaNomes lista, ArrayList<String> nomes, String subString) {
        this.nomes= nomes;
        this.lista= lista;
        this.subString= subString;     // Todos as strings são consideradas minúsculas
        this.subString= this.subString.toLowerCase();

        this.pesquisa= new TreeSet<>();
    }
    /**
     * Método run da classe Pesquisa em que está contido todo o código necessário
     * para implementar adequadamente o sistema de pesquisas em ArrayList e inserir
     * os resultados em um ListaNomes. Não há concorrência e compartilhamento de
     * recursos, por isso o uso de ArrayList e Strings.
     */
    @Override
    public void run() {
        Iterator iterator= this.nomes.iterator();   // Iterator de um ArrayList
        String nomeAtor, string;

        while((iterator.hasNext()) && (!Thread.interrupted())) {
            nomeAtor= ((String)iterator.next());    // Se a string vinda do ArrayList contém a
                                                    // substring de busca, é adicionada ao SortedSet
            string= nomeAtor;
            string= string.toLowerCase();   // As strings são consideradas minúsculas

            if (string.contains(this.subString)) this.pesquisa.add(nomeAtor);
        }
        Object[] resultadoPesquisa= this.pesquisa.toArray();
        this.pesquisa.clear();

        this.lista.setListData(resultadoPesquisa);  // ListaNomes recebe o vetor resultado da busca
    }
}
