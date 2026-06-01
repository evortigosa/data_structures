/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package graphickb;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import javax.swing.*;
import tadgrafo.Grafo;

/**
 * Classe que monta o Frame principal do programa tratando suas ações para
 * permitir consultar os número do mundo de Kevin Bacon.
 *
 * @author EVANDRO
 */
public final class Frame extends JFrame {
    private Grafo grafo;
    private ArrayList<String> atores;
    private Menu topMenu;              // Barra de Menu
    private JLabel label;
    private ListaNomes lista;          // Lista com os nomes disponíveis
    private JButton button1, button2, button3, button4;
    private JPanel panel;
    private JTextArea textArea;        // TextArea que exibirá o resultado das pesquisas

    /**
     * Construtor da classe Frame.
     * @param grafo Grafo contendo nomes de atores (armazenandos nos vértices)
     * interconectados por filmes (armazenandos nas arestas)
     * @param atores ArrayList contendo o nome de todos os atores inseridos no grafo
     */
    public Frame(Grafo grafo, ArrayList<String> atores) {
        super("The KB World");

        this.grafo= grafo;
        this.atores= atores;

        this.topMenu= new Menu(Frame.this);

        this.label= new JLabel("Nome do ator:");
        this.label.setBounds(110, 55, 90, 15);
        this.label.setToolTipText("Informe um ator para verificar seu KB");
        this.add(this.label);

        this.lista= new ListaNomes(200, 50, Frame.this, this.atores);

        ButtonHandler bHandler= new ButtonHandler();  // Listener de ação dos botões

        this.button1= new JButton("Verificar KB");
        this.button1.setBounds(490, 46, 103, 28);
        this.button1.addActionListener(bHandler);
        this.add(this.button1);

        this.button2= new JButton("Estatísticas");
        this.button2.setBounds(490, 86, 103, 28);     // Definição dos botões do frame e também
        this.button2.addActionListener(bHandler);     // o registro de seus Listeners
        this.add(this.button2);

        this.button3= new JButton("Limpar");
        this.button3.setBounds(205, 315, 103, 28);
        this.button3.addActionListener(bHandler);
        this.add(this.button3);

        this.button4= new JButton("Salvar");
        this.button4.setBounds(330, 315, 103, 28);
        this.button4.setEnabled(false);
        this.button4.addActionListener(bHandler);
        this.add(this.button4);

        this.textArea= new JTextArea(7, 65);          // Definição do TextArea de resultados - read-only
        this.textArea.setBounds(1, 1, 725, 110);      // Posiçao relativa ao JPanel
        this.textArea.setEnabled(false);
        this.textArea.setDisabledTextColor(Color.BLACK);
        this.textArea.setFont(new Font("Consolas", 0, 13));

        this.panel= new JPanel();
        this.panel.setBounds(27, 163, 728, 113);
        this.panel.setBorder(BorderFactory.createEtchedBorder());
        this.panel.setLayout(null);
        this.panel.add(this.textArea);
        this.add(this.panel);

        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            SwingUtilities.updateComponentTreeUI(Frame.this);   // Frame é inicializado com o LookAndFeel do sistema
        }
        catch (ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException except) {
            // Não gera exceções
        }
        this.setSize(new Dimension(800, 495));
        this.setLocationRelativeTo(null);        // Posicionamento do Frame no centro da tela
        this.setLayout(null);

        this.lista.setTextFieldFocus();          // TextField de ListaNomes recebe o foco inicial
    }
    /**
     * Define o texto do componente TextField de Lista. Se o texto for vazio,
     * tem o efeito de simplesmente apagar o texto antigo.
     * @param text Novo texto a ser definido
     */
    public void setListaTextField(String text) {
        this.lista.setTextField(text);
    }
    /**
     * Solicita que o componente TextField de Lista receba o foco.
     */
    public void setListaTextFieldFocus() {
        this.lista.setTextFieldFocus();
    }
    /**
     * Define o texto do componente TextArea do Frame. Se o texto for vazio,
     * tem o efeito de simplesmente apagar o texto antigo.
     * @param text Novo texto a ser definido
     */
    public void setTextArea(String text) {
        this.textArea.setText(text);
    }
    /**
     * Retorna o texto contido no componente TextArea do Frame.
     * @return String com o texto contido no campo
     */
    public String getTextArea() {
        return this.textArea.getText();
    }
    /**
     * Define se o botão Salvar está habilitado. Um componente que estiver
     * habilitado pode responder a entrada do usuário, enquanto um componente
     * que não é habilitado não pode responder a entrada do usuário.
     * @param enabled Verdadeiro se o componente deve ser ativado, falso caso contrário
     */
    public void setSaveButtonEnabled(boolean enabled) {
        this.button4.setEnabled(enabled);
    }

    /**
     * Classe que implementa a interface Listener para receber eventos de ação dos
     * botões do Frame.
     * O objeto Listener criado a partir dessa classe é registrado utilizando o método
     * addActionListener.
     * Quando uma ação ocorre, o método actionPerformed daquele objeto é invocado.
     */
    private class ButtonHandler implements ActionListener {

        /**
         * Método chamado quando ocorre uma ação em um dos botões.
         * @param event Evento de ação do botão
         */
        @Override
        public void actionPerformed(ActionEvent event) {
            lista.setVisibleList(false);

            if (event.getSource()== button1) this.setActionCalcularKB();
            else if (event.getSource()== button2) this.setActionEstatisticas();
            else if (event.getSource()== button3) this.setActionLimpar();
            else if (event.getSource()== button4) this.setActionSalvar();
        }
        /**
         * Define a ação do botão "Verificar", que calcula e exibe o número de KB,
         * assim como o caminho mais curto (formado por filmes) que leva o ator
         * escolhido até Kevin Bacon.
         */
        public void setActionCalcularKB() {

            try {
                String ator= lista.getTextField();
                String caminho= grafo.caminhoMinimo("Bacon, Kevin", ator);

                textArea.setText(caminho);
                topMenu.setSaveItemEnabled(true);     // Com o resultado da pesquisa é
                button4.setEnabled(true);             // possível salvar um arquivo texto
            }
            catch (NullPointerException | NoSuchElementException except) {
                JOptionPane.showMessageDialog(null, except.getMessage(), "Erro!", 0);
                lista.setTextFieldFocus();
            }
        }
        /**
         * Define a ação do botão "Estatísticas", que calcula e exibe as informações
         * estatísticas relativas ao mundo de KB, como média de todos os números de
         * KB assim como seu desvio padrão.
         */
        public void setActionEstatisticas() {

            try {
                double mediaKB= grafo.getMedia("Bacon, Kevin");
                double desvioPadraoKB= grafo.getDesvioPadrao("Bacon, Kevin", mediaKB);

                String dados= ("Seria o mundo de Kevin Bacon pequeno?\n\nMédia de todos os KBs do grafo: " + String.format("%.10f", mediaKB)
                        + "\n                 Desvio padrão: " + String.format("%.10f", desvioPadraoKB) + "\n");
                textArea.setText(dados);
                topMenu.setSaveItemEnabled(true);     // Com o resultado das estatísticas é
                button4.setEnabled(true);             // possível salvar um arquivo texto
            }
            catch (Exception except) {
                JOptionPane.showMessageDialog(null, "Impossível realizar a operação solicitada!", "Erro!", 0);
            }
        }
        /**
         * Define a ação do botão "Limpar", em que os campos de pesquisa e resultado
         * são limpos para novas pesquisas.
         */
        public void setActionLimpar() {
            lista.setTextField("");
            textArea.setText("");
            topMenu.setSaveItemEnabled(false);   // Item de menu "Salvar" e botão
            button4.setEnabled(false);           // "Salvar" são desabilitados
            lista.setTextFieldFocus();
        }
        /**
         * Define a ação do botão "Salvar", em que os dados resultantes da pesquisa
         * pelo nome de um ator/atriz no mundo de KB são salvos em um arquivo texto
         * identificado com o nome do ator/atriz.
         */
        public void setActionSalvar() {
            String dados= textArea.getText();

            SalvarDados save= new SalvarDados(dados);
            save.Salvar();
        }
    }
}
