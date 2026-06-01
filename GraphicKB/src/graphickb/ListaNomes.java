/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package graphickb;

import java.awt.event.*;
import java.util.ArrayList;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

/**
 * Classe responsável por implementar e tratar os eventos de um mecanismo que permite
 * ao usuário digitar e/ou escolher um nome de uma Lista que contenha os nomes disponíveis.
 * Este mecanismo é composto por um TextField e um List trabalhando em conjunto, enquanto
 * o usuário realiza a digitação, o sistema atualiza a Lista de nomes para que ela exiba
 * apenas os nomes que contenham a substring inserida.
 *
 * @author EVANDRO
 */
public class ListaNomes {
    private Frame frame;
    private ArrayList<String> nomes;
    private JTextField text;           // Campo de texto em que o usuário poderá inserir um nome
    private JList list;                // Lista que conterá os nomes disponíveis
    private JScrollPane listScroller;  // JScrollPane que receberá a Lista

    /**
     * Construtor da classe ListaNomes.
     * @param x Inteiro com a nova coordenada-x desse componente
     * @param y Inteiro com a nova coordenada-y desse componente
     * @param frame Frame em que o componente será inserido
     * @param nomes ArrayList de strings que servirá de base para a lista
     */
    public ListaNomes(int x, int y, Frame frame, ArrayList<String> nomes) {
        this.frame= frame;
        this.nomes= nomes;

        TextHandler tHandler= new TextHandler();
        KeyHandler kHandler= new KeyHandler();             // Listeners de texto, tecla e mouse
        MouseHandler moHandler= new MouseHandler();

        this.text= new JTextField();
        this.text.setBounds(x, y, 250, 25);
        this.text.setFocusable(true);
        this.text.getDocument().addDocumentListener(tHandler);  // Componente TextField possui Listeners
        this.text.addKeyListener(kHandler);                     // para alterações em seu documento e
        this.frame.add(this.text);                              // eventos de teclado

        this.list= new JList();
        this.list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        this.list.setLayoutOrientation(JList.VERTICAL);         // Definição do modelo e orientação da lista
        this.list.setVisibleRowCount(-1);
        this.list.addKeyListener(kHandler);                     // Componente List possui Listeners para
        this.list.addMouseMotionListener(moHandler);            // eventos de teclado, mouse e movimento
        this.list.addMouseListener(moHandler);                  // de mouse

        this.listScroller= new JScrollPane(this.list);
        this.listScroller.setBounds(x, (y+ 24), 250, 130);      // Lista inserida em um Painel de Rolagem
        this.listScroller.setVisible(false);
        this.frame.add(this.listScroller);
    }
    /**
     * Define o texto do componente TextField de ListaNomes. Se o texto for
     * vazio, tem o efeito de simplesmente apagar o texto antigo.
     * @param text Novo texto a ser definido
     */
    public void setTextField(String text) {
        this.text.setText(text);
    }
    /**
     * Retorna o texto contido no componente TextField de ListaNomes.
     * @return String com o texto contido no campo
     */
    public String getTextField() {
        return this.text.getText();
    }
    /**
     * Solicita que o componente TextField de ListaNomes receba o foco.
     */
    public void setTextFieldFocus() {
        this.text.grabFocus();
    }
    /**
     * Define os itens read-only contidos na Lista de ListaNomes.
     * @param data Vetor de objetos (Strings) que contém os itens a serem exibidos
     * na lista
     */
    public void setListData(Object[] data) {
        this.list.setListData(data);
    }
    /**
     * Torna o componente Lista de ListaNomes visível ou invisível.
     * @param visible Verdadeiro para tornar o componente visível, falso para torná-lo
     * invisível
     */
    public void setVisibleList(boolean visible) {
        this.listScroller.setVisible(visible);
    }

    /**
     * Classe que implementa a interface Listener para se registrar e receber
     * notificações de alteração em um documento de texto.
     */
    private class TextHandler implements DocumentListener {

        /**
         * Notifica que houve uma inserção no documento em TextField e atualiza
         * a Lista.
         * @param event Evento de documento
         */
        @Override
        public void insertUpdate(DocumentEvent event) {
            this.atualizaLista();
        }
        /**
         * Notifica que uma parte do documento em TextField foi removido e atualiza
         * a Lista.
         * @param event Evento de documento
         */
        @Override
        public void removeUpdate(DocumentEvent event) {

            if (text.getText().compareTo("")!= 0) this.atualizaLista();
            else listScroller.setVisible(false);
        }
        /**
         * Notifica que um atributo ou conjunto de atributos foram alterados.
         * Não houve necessidade de desenvolver este método.
         * @param event Evento de documento
         */
        @Override
        public void changedUpdate(DocumentEvent event) { }
        /**
         * Atualiza e reexibe os itens da Lista quando houve uma inserção ou
         * remoção de uma parte do documento em TextField.
         */
        private void atualizaLista() {
            String subString= text.getText();

            Runnable pesquisa= new Pesquisa(ListaNomes.this, nomes, subString);
            Thread thread= new Thread(pesquisa);      // A pesquisa é feita por Threads para que não
            thread.start();                           // haja interrupções no fluxo de digitação

            listScroller.setVisible(true);
        }
    }

    /**
     * Classe que implementa a interface Listener para receber eventos de teclado (teclas).
     * O objeto Listener criado a partir dessa classe é registrado usando o método addKeyListener.
     * Um evento de teclado é gerado quando uma tecla é pressionada, liberada, ou digitada.
     */
    private class KeyHandler implements KeyListener {

        /**
         * Método chamado quando uma tecla foi digitada. Não houve necessidade de
         * desenvolver este método.
         * @param event Evento de teclado
         */
        @Override
        public void keyTyped(KeyEvent event) { }
        /**
         * Método chamado quando uma tecla foi pressionada.
         * @param event Evento de teclado
         */
        @Override
        public void keyPressed(KeyEvent event) {

            if (event.getSource()== text) {           // Componente TextField
                if (event.getKeyCode()== 38) {        // Seta pra cima
                    list.setSelectedIndex(list.getModel().getSize()- 1);
                    list.ensureIndexIsVisible(list.getSelectedIndex());
                    list.grabFocus();
                }
                else if (event.getKeyCode()== 40) {   // Seta pra baixo
                    list.setSelectedIndex(0);
                    list.ensureIndexIsVisible(list.getSelectedIndex());
                    list.grabFocus();
                }
            }
            else if (event.getSource()== list) {      // Componente List
                if (event.getKeyCode()== 27) {        // Esc
                    listScroller.setVisible(false);
                    text.grabFocus();
                }
                else if (event.getKeyCode()== 10) {   // Enter
                    text.setText((String)list.getSelectedValue());
                    listScroller.setVisible(false);
                }
            }
        }
        /**
         * Método chamado quando uma tecla foi liberada. Não houve necessidade de
         * desenvolver este método.
         * @param event Evento de teclado
         */
        @Override
        public void keyReleased(KeyEvent event) { }
    }

    /**
     * Classe que implementa as interfaces Listener para receber eventos de movimento
     * do mouse em um componente e também eventos de mouse (pressionar, liberar,
     * clicar, entrada e saída) em um componente.
     * O objeto de Listener criado a partir dessa classe é registrado usando os métodos
     * addMouseMotionListener e addMouseListener.
     */
    private class MouseHandler implements MouseMotionListener, MouseListener {

        /**
         * Método chamado quando um botão do mouse é pressionado em um componente
         * e depois arrastado. Não houve necessidade de desenvolver este método.
         * @param event Evento de movimento do mouse
         */
        @Override
        public void mouseDragged(MouseEvent event) { }
        /**
         * Método chamado quando o cursor do mouse foi movido em um componente, mas
         * nenhum botão foi pressionado.
         * @param event Evento de movimento do mouse
         */
        @Override
        public void mouseMoved(MouseEvent event) {
            int indice= list.locationToIndex(event.getPoint());

            list.grabFocus();
            list.setSelectedIndex(indice);
        }
        /**
         * Método chamado quando um botão do mouse foi clicado (pressionado e liberado)
         * em um componente.
         * @param event Evento de mouse
         */
        @Override
        public void mouseClicked(MouseEvent event) {
            text.setText((String)list.getSelectedValue());
            listScroller.setVisible(false);
        }
        /**
         * Método chamado quando um botão do mouse foi pressionado em um componente.
         * Não houve necessidade de desenvolver este método.
         * @param event Evento de mouse
         */
        @Override
        public void mousePressed(MouseEvent event) { }
        /**
         * Método chamado quando um botão do mouse foi liberado em um componente.
         * Não houve necessidade de desenvolver este método.
         * @param event Evento de mouse
         */
        @Override
        public void mouseReleased(MouseEvent event) { }
        /**
         * Método chamado quando o mouse entra em um componente. Não houve necessidade
         * de desenvolver este método.
         * @param event Evento de mouse
         */
        @Override
        public void mouseEntered(MouseEvent event) { }
        /**
         * Método chamado quando o mouse sai de um componente. Não houve necessidade
         * de desenvolver este método.
         * @param event Evento de mouse
         */
        @Override
        public void mouseExited(MouseEvent event) { }
    }
}
