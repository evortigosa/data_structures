/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package graphickb;

import java.io.*;
import javax.swing.JOptionPane;

/**
 * Classe responsável por salvar as informações obtidas pelo usuário em buscas
 * feitas por nomes de atores/atrizes contidos no mundo de KB.
 *
 * @author EVANDRO
 */
public final class SalvarDados {
    private String conteudo, fileName, filePath;

    /**
     * Construtor da classe SalvarDados.
     * @param conteudo String contendo as informações que serão salvas em arquivo
     * texto
     */
    public SalvarDados(String conteudo) {
        this.conteudo= conteudo;
    }
    /**
     * Método privado da classe SalvarDados responsável por extrair o nome do
     * arquivo texto que será salvo de seu próprio conteúdo.
     */
    private void nomeArquivo() {
                                       // Nesse caso foram recebidas as estatísticas do mundo de KB
        if (this.conteudo.contains("KBs do grafo")) this.fileName= "KB_world_info.txt";
        else {
            int limite= this.conteudo.indexOf("\"", 1);    // Aqui foram recebidas informações sobre um ator de
                                                           // onde é extraído seu nome para nomear o arquivo
            String auxiliar= this.conteudo.substring(1, limite);

            auxiliar= auxiliar.replace(" ", "_");          // Nome do ator tem espaços em branco substituídos por
            auxiliar= auxiliar.replaceAll("\\W", "");      // caracteres underline. A pontuação é retirada

            this.fileName= (auxiliar + ".txt");
        }
    }
    /**
     * Método privado da classe SalvarDados responsável por criar, caso não exista,
     * um arquivo texto com o nome capturado do conteúdo recebido pelo construtor.
     * @throws IOException no caso de não ser possível criar o arquivo
     */
    private void criaArquivo() throws IOException {
        File fArquivo= new File(this.fileName);

        try {                     // Se não existir, arquivo é criado
            if ((fArquivo.exists())!= true) fArquivo.createNewFile();

            this.filePath= fArquivo.getCanonicalPath();
        }                         // Endereço do arquivo é capturado como informação
        catch (IOException except) {
            throw new IOException("Arquivo não pode ser criado!");
        }
    }
    /**
     * Método responsável por salvar as informações obtidas pelo usuário em buscas
     * feitas por nomes de atores/atrizes contidos no mundo de KB.
     */
    public void Salvar() {

        try {
            this.nomeArquivo();   // Extração do nome do arquivo de seu conteúdo
            this.criaArquivo();   // Criação, caso necessário, de novo arquivo texto

            PrintWriter fArquivo= new PrintWriter(new BufferedWriter(new FileWriter(this.fileName)));
                                  // Objeto de escrita buferizada de caracteres
            fArquivo.println(this.conteudo);

            fArquivo.flush();
            fArquivo.close();     // Arquivo pronto é fechado e tem seu nome e endereço em disco exibidos para o usuário

            JOptionPane.showMessageDialog(null, ("\"" + this.fileName + "\" salvo em\n" + this.filePath), "Informação", 1);
        } catch (IOException except) {
            JOptionPane.showMessageDialog(null, except.getMessage(), "Erro!", 0);
        }
    }
}
