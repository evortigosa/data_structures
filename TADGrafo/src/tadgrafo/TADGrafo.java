/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import java.util.NoSuchElementException;
import java.util.Scanner;

/**
 * Universidade de São Paulo - USP
 * Instituto de Ciências Matemáticas e de Computação - ICMC
 *
 * 2º Trabalho Prático - TAD Grafo em Java
 * SCC0204 - Programação Orientada a Objetos
 * Professor Lucas Antiqueira
 *
 * @author Evandro Scudeleti Ortigossa, nUSP 6793135 - Maio de 2012.
 * @version 1.0.0
 */

/**
 * Classe que contém o método main com instância e uso de métodos de manipulação
 * do TAD Grafo.
 *
 * @author EVANDRO
 */
public class TADGrafo {

    /**
     * Método principal do programa
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Scanner entrada= new Scanner(System.in);
        String caminho, opcao= "n";
        Grafo grafo= new Grafo();

        TrataArquivo arquivo= new TrataArquivo();
        arquivo.leArquivo("input-top-grossing.txt", grafo);     // Leitura do arquivo fornecido e "montagem" do grafo

        System.out.println("Bem-vindo ao mundo de Kevin Bacon!\n");

        while (opcao.equalsIgnoreCase("n")) {
            System.out.print("1 - Calcular o KB de um ator\n2 - Consultar as estatísticas do mundo de KB\n3 - Sair\n\nSua escolha: ");
            opcao= entrada.nextLine();           // Escolha de uma das opções do menu inicial

            if (opcao.equalsIgnoreCase("1")) {
                opcao= "s";

                while (opcao.equalsIgnoreCase("s")) {
                    System.out.print("\nInforme um ator para verificar seu KB: ");
                    String atorDestino= entrada.nextLine();     // Usuário informa o nome de um ator
                    System.out.print("\n");                     // para calcular seu número de KB

                    try {                                                                // Cálculo do KB e busca do caminho mais
                        caminho= grafo.caminhoMinimo("Bacon, Kevin", atorDestino);       // curto (formado por filmes) que leva
                        System.out.print(caminho);                                       // o ator escolhido até Kevin Bacon
                    }
                    catch (NullPointerException | NoSuchElementException erro) {
                        System.err.println(erro.getMessage());
                    }        // Nome de ator escolhido pode não existir no arquivo

                    System.out.print("\nContinuar verificando KBs? (s/n) ");
                    opcao= entrada.nextLine();
                }
            }
            else if (opcao.equals("2")) {        // Verificação de estatísticas do Mundo de Kevin Bacon
                try {
                    double mediaKB= grafo.getMedia("Bacon, Kevin");
                    double desvioPadraoKB= grafo.getDesvioPadrao("Bacon, Kevin", mediaKB);

                    System.out.println("\nSeria o mundo de Kevin Bacon pequeno?");
                    System.out.format("\nMédia de todos os KBs do grafo: %.10f"
                            + "\n                 Desvio padrão: %.10f\n", mediaKB, desvioPadraoKB);
                }
                catch (Exception erro) {
                    System.err.println("\nImpossível realizar a operação solicitada!");
                }  // Exceção gerada quando o arquivo não foi corretamente lido e, portanto, não é possível manipular os dados
            }

            if (!opcao.equals("3")) {
                System.out.print("\nDeseja sair do programa? (s/n) ");
                opcao= entrada.nextLine();
                System.out.print("\n");
            }
        }
        grafo.finalizaGrafo();         // Grafo é finalizado e programa encerrado
    }
}
