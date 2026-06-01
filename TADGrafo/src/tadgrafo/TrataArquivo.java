/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.NoSuchElementException;
import javax.swing.JOptionPane;

/**
 * Classe responsável pela leitura e tratamento de um arquivo texto, conforme as
 * especificações do trabalho, contendo uma lista de filmes e respectivas listas
 * de atores que trabalharam em cada filme que serão inseridos em um grafo.
 *
 * @author EVANDRO
 */
public final class TrataArquivo {
    private String filme;
    private String[] tokens;

    /**
     * Método responsável pela leitura e processamento em memória principal do
     * arquivo com uma lista de filmes e atores.
     * Cada linha do arquivo é formada pelo nome de um filme, seguido por '/' e
     * pela lista de atores que nele atuaram (também separados por '/').
     * @param arquivo String com o nome/local do arquivo
     * @param grafo Grafo que receberá vértices e arestas conforme os dados do arquivo
     */
    public void leArquivo(String arquivo, Grafo grafo) {
        String linha;

        try {      // Instancia-se o objeto fArquivo que oferece o método de leitura readLine()
            BufferedReader fArquivo= new BufferedReader(new FileReader(arquivo));

            while((linha= fArquivo.readLine())!= null) {   // Faz-se um loop linha a linha no arquivo enquanto o retorno
                this.tokens= linha.split("/");             // for diferente de null. O método readLine devolve a linha
                                                           // corrente de leitura para a variável linha
                this.filme= this.tokens[0];

                this.processaDados(grafo);
            }
            fArquivo.close();
        }
        catch (IOException erro) {
            JOptionPane.showMessageDialog(null, erro.getMessage());
        }
    }
    /**
     * Método responsável pelo processamento dos dados obtidos do arquivo lido,
     * montando um grafo com vértices (atores) e arestas ligando os atores que
     * atuaram em um mesmo filme.
     * O arquivo "input-top-grossing.txt" gera um grafo com 8251 vértices interligados
     * por 314727 arestas. Portanto, a execução completa deste método pode levar
     * alguns minutos.
     * @param grafo Grafo que receberá vértices e arestas conforme os dados do arquivo
     */
    public void processaDados(Grafo grafo) {
        int tamanho= this.tokens.length;
                                            // Todos os atores recebidos são inseridos no grafo como vértices
        for (int i= 1; i< tamanho; i++) grafo.insereVertice(this.tokens[i]);

        for (int i= 1; i< tamanho; i++) {             // Arestas ligando os atores que atuaram em um mesmo filme são
            for (int j= (i+ 1); j< tamanho; j++) {    // inseridas apartir da lista obtida em cada linha do arquivo
                try {
                    grafo.insereAresta(this.filme, this.tokens[i], this.tokens[j]);
                }
                catch (IllegalArgumentException | NoSuchElementException erro) {
                    System.err.println(erro.getMessage());      // Para o arquivo dado, esta exceção não ocorre
                }
            }
        }
    }
}
