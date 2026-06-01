/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package graphickb;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

/**
 * Classe responsável por implementar e tratar as ações geradas em um Menu de opções
 * exibido no topo do Frame.
 *
 * @author EVANDRO
 */
public class Menu {
    private Frame frame;
    private JMenuBar menuBar;
    private JMenu fileMenu, viewsMenu, helpMenu;      // Campos do Menu
    private JMenuItem clearItem, saveItem, exitItem;  // Itens contidos no campo Arquivo
    private JMenuItem metalItem, nimbusItem, cdeItem, windowsItem, wClassicItem;    // Itens contidos no campo Estilo
    private JMenuItem aboutItem;                      // Item contido no campo Ajuda

    /**
     * Construtor da classe Menu.
     * @param frame Frame em que o componente será inserido
     */
    public Menu(Frame frame) {
        this.frame= frame;

        this.menuBar= new JMenuBar();
        this.frame.setJMenuBar(this.menuBar);

        this.fileMenu= new JMenu("Arquivo");
        this.menuBar.add(this.fileMenu);
        this.viewsMenu= new JMenu("Estilo");          // Definição dos campos do Menu
        this.menuBar.add(this.viewsMenu);
        this.helpMenu= new JMenu("Ajuda");
        this.menuBar.add(this.helpMenu);

        MenuHandler mHandler= new MenuHandler();      // Listener de ação dos itens de menu

        this.clearItem= new JMenuItem("Limpar campos");
        this.clearItem.addActionListener(mHandler);
        this.fileMenu.add(this.clearItem);
        this.saveItem= new JMenuItem("Salvar");
        this.saveItem.setEnabled(false);              // Definição dos itens contidos no campo de menu Arquivo
        this.saveItem.addActionListener(mHandler);    // e também o registro de seus Listeners
        this.fileMenu.add(this.saveItem);
        this.fileMenu.addSeparator();
        this.exitItem= new JMenuItem("Sair");
        this.exitItem.addActionListener(mHandler);
        this.fileMenu.add(this.exitItem);

        this.metalItem= new JMenuItem("Metal");
        this.metalItem.addActionListener(mHandler);
        this.viewsMenu.add(this.metalItem);
        this.nimbusItem= new JMenuItem("Nimbus");
        this.nimbusItem.addActionListener(mHandler);
        this.viewsMenu.add(this.nimbusItem);          // Definição dos itens contidos no campo de menu Estilo
        this.cdeItem= new JMenuItem("CDE/Motif");     // e também o registro de seus Listeners
        this.cdeItem.addActionListener(mHandler);
        this.viewsMenu.add(this.cdeItem);
        this.windowsItem= new JMenuItem("Windows");
        this.windowsItem.addActionListener(mHandler);
        this.viewsMenu.add(this.windowsItem);
        this.wClassicItem= new JMenuItem("Windows Classic");
        this.wClassicItem.addActionListener(mHandler);
        this.viewsMenu.add(this.wClassicItem);

        this.aboutItem= new JMenuItem("Sobre...");    // Definição do item contido no campo de menu Ajuda
        this.aboutItem.addActionListener(mHandler);   // e também o registro de seu Listener
        this.helpMenu.add(this.aboutItem);
    }
    /**
     * Define se o componente Salvar do campo Arquivo do menu está habilitado.
     * Um componente que estiver habilitado pode responder a entrada do usuário,
     * enquanto um componente que não é habilitado não pode responder a entrada
     * do usuário.
     * @param enabled Verdadeiro se o componente deve ser ativado, falso caso contrário
     */
    public void setSaveItemEnabled(boolean enabled) {
        this.saveItem.setEnabled(enabled);
    }

    /**
     * Classe que implementa a interface Listener para receber eventos de ação do Menu.
     * O objeto Listener criado a partir dessa classe é registrado utilizando o método
     * addActionListener.
     * Quando uma ação ocorre, o método actionPerformed daquele objeto é invocado.
     */
    private class MenuHandler implements ActionListener {

        /**
         * Método chamado quando ocorre uma ação de menu.
         * @param event Evento de ação do menu
         */
        @Override
        public void actionPerformed(ActionEvent event) {

            if (event.getSource()== clearItem) this.setActionLimpar();              // Itens contidos no campo Arquivo
            else if (event.getSource()== saveItem) this.setActionSalvar();
            else if (event.getSource()== exitItem) this.setActionSair();
            else if (event.getSource()== metalItem) this.setLookAndFeel("Metal");   // Itens contidos no campo Estilo
            else if (event.getSource()== nimbusItem) this.setLookAndFeel("Nimbus");
            else if (event.getSource()== cdeItem) this.setLookAndFeel("Motif");
            else if (event.getSource()== windowsItem) this.setLookAndFeel("Windows");
            else if (event.getSource()== wClassicItem) this.setLookAndFeel("Classic");
            else if (event.getSource()== aboutItem) this.setActionSobre();          // Item contido no campo Ajuda
        }
        /**
         * Define a ação do item de menu "Limpar", em que os campos de pesquisa
         * e resultado são limpos para novas pesquisas.
         */
        public void setActionLimpar() {
            frame.setListaTextField("");
            frame.setTextArea("");
            setSaveItemEnabled(false);           // Item de menu "Salvar" e botão
            frame.setSaveButtonEnabled(false);   // "Salvar" são desabilitados
            frame.setListaTextFieldFocus();
        }
        /**
         * Define a ação do item de menu "Salvar", em que os dados resultantes da
         * pesquisa pelo nome de um ator/atriz no mundo de KB são salvos em um
         * arquivo texto identificado com o nome do ator/atriz.
         */
        public void setActionSalvar() {
            String dados= frame.getTextArea();

            SalvarDados save= new SalvarDados(dados);
            save.Salvar();
        }
        /**
         * Define a ação do item de menu "Sair", fechando o Frame e finalizando
         * a execução do programa na Java Virtual Machine.
         */
        public void setActionSair() {
            System.exit(0);
        }
        /**
         * Define a ação dos itens de menu em "Estilo", alterando o LookAndFeel
         * do programa conforme a escolha do usuário.
         * @param lookAndFeel String com o nome do LookAndFeel desejado (Metal, Nimbus,
         * Motif, Windows ou Classic)
         */
        public void setLookAndFeel(String lookAndFeel) {
            String newLook= "";
                                  // Construção da string completa de especificação do nome de classe que implementa o LookAndFeel
            if (lookAndFeel.equals("Metal") || lookAndFeel.equals("Nimbus"))
                newLook= ("javax.swing.plaf." + lookAndFeel.toLowerCase() + "." + lookAndFeel + "LookAndFeel");
            else if (lookAndFeel.equals("Motif") || lookAndFeel.equals("Windows"))
                newLook= ("com.sun.java.swing.plaf." + lookAndFeel.toLowerCase() + "." + lookAndFeel + "LookAndFeel");
            else if (lookAndFeel.equals("Classic"))
                newLook= ("com.sun.java.swing.plaf.windows.Windows" + lookAndFeel + "LookAndFeel");

            try {
                UIManager.setLookAndFeel(newLook);
                SwingUtilities.updateComponentTreeUI(frame);    // Atualização do LookAndFeel do programa
            }
            catch (ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException except) {
                // Apenas ignora, não gera exceções
            }
        }
        /**
         * Define a ação do item de menu "Sobre" que mostra em um JOptionPane informações
         * gerais relativas ao sistema GraphicKB.
         */
        public void setActionSobre() {
            String about= ("Universidade de São Paulo - USP\nInstituto de Ciências Matemáticas e de Computação - ICMC      \n\n"
                    + "3º Trabalho Prático - Mundo de KB Gráfico em Java\nSCC0204 - Programação Orientada a Objetos\n"
                    + "Professor Lucas Antiqueira\n\nEvandro Scudeleti Ortigossa, nUSP 6793135\n"
                    + "Version 2.0.0\nCopyright (C) 2012 - All rights reserved.\n\n");

            JOptionPane.showMessageDialog(null, about, "Sobre", 1);
        }
    }
}
