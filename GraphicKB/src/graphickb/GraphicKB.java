/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package graphickb;

import java.awt.Dimension;
import java.util.ArrayList;
import javax.swing.JFrame;
import recebearquivo.RecebeArquivo;
import tadgrafo.Grafo;

/**
 * Universidade de São Paulo - USP
 * Instituto de Ciências Matemáticas e de Computação - ICMC
 *
 * 3º Trabalho Prático - Mundo de KB Gráfico em Java
 * SCC0204 - Programação Orientada a Objetos
 * Professor Lucas Antiqueira
 *
 * @author Evandro Scudeleti Ortigossa, nUSP 6793135 - Junho de 2012.
 * @version 2.0.0
 */

/**
 * Classe que contém o método main.
 *
 * @author EVANDRO
 */
public class GraphicKB {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Grafo grafo= new Grafo();
        RecebeArquivo arquivo= new RecebeArquivo();
                                       // Leitura do arquivo de entrada e montagem do grafo
        ArrayList<String> atores= ((ArrayList<String>)arquivo.leArquivo("input-top-grossing.txt", grafo));
                                       // ArrayList contendo o nome de todos os atores inseridos no grafo
        Frame kbFrame= new Frame(grafo, atores);

        kbFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        kbFrame.setMinimumSize(new Dimension(800, 465));   // Definição do tamanho mínimo do frame
        kbFrame.setVisible(true);
    }
}
