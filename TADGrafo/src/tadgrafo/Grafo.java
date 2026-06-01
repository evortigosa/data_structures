/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tadgrafo;

import java.util.NoSuchElementException;
import tadgrafo.Aresta.NoAresta;
import tadgrafo.Vertice.NoVertice;

/**
 * Classe Grafo capaz de armazenar um conjunto de vértices interconectados por
 * um conjunto de arestas, além de quaisquer outras informações necessárias.
 * Para esse desenvolvimento, foi adotada a estratégia de Listas de Adjacências.
 *
 * @author EVANDRO
 */
public class Grafo {
    private Vertice vertices= null;
    private Aresta arestas= null;

    /**
     * Construtor da classe Grafo.
     */
    public Grafo() {
        this.vertices= new Vertice();
        this.arestas= new Aresta();
    }
    /**
     * Método insere um vértice no grafo armazenando o elemento ator.
     * É feita uma verificação se o ator já foi inserido no grafo (para que não
     * haja repetições). Se não foi, é inserido um vértice com o nome do ator
     * na lista de vértices do grafo.
     * @param ator String com o nome do ator inserido
     */
    public void insereVertice(String ator) {

        if (this.vertices.existeElemento(ator)== null) this.vertices.insereNaLista(ator);
    }
    /**
     * Método que remove do grafo um vértice e todas as arestas incidentes a ele.
     * @param ator String com o nome do ator removido
     * @throws NoSuchElementException no caso de não haver vértices a serem removidos
     */
    public void deletaVertice(String ator) {

        NoVertice vertice= ((NoVertice)this.vertices.existeElemento(ator));

        if (this.vertices.listaVazia()) throw new NoSuchElementException("Não há vértices no grafo!");
        else if (vertice== null) throw new NoSuchElementException("Vértice não encontrado!");
        else {
            while(vertice.getListaAdjacencias()!= null) this.deletaAresta(vertice.getListaAdjacencias().getAresta());
                                                      // Remove todas as arestas incidentes do vértice removido
            this.vertices.removeDaLista(vertice);     // Remoção do vértice da lista de vértices
        }
    }
    /**
     * Retorna um inteiro com o número de elementos contidos na lista de vértices
     * do grafo.
     * @return Número de vértices contidos no grafo
     */
    public int numeroVertices() {
        return this.vertices.tamanhoLista();
    }
    /**
     * Método que retorna o vértice final da aresta separado do vértice atual.
     * @param vAtual Vértice atual em que se buscará seu oposto
     * @param aresta Aresta incidente ao vértice atual
     * @return Vértice final da aresta separado do vértice atual
     */
    public NoVertice verticeOposto(NoVertice vAtual, NoAresta aresta) {
        NoVertice vOposto= aresta.getVerticeA();

        if (vAtual== vOposto) return aresta.getVerticeB();

        return vOposto;
    }
    /**
     * Método insere uma nova aresta não-dirigida com vértices finais rotulados
     * por atorA e atorB e armazena o elemento filme.
     * @param filme String com o nome do filme armazenado pela nova aresta
     * @param atorA String com o rótulo de um dos vértices finais da nova aresta
     * @param atorB String com o rótulo do outro vértice final da nova aresta
     * @throws IllegalArgumentException no caso dos vértices finais serem o mesmo vértice
     * @throws NoSuchElementException no caso de pelo menos um dos vértices não existir no grafo
     */
    public void insereAresta(String filme, String atorA, String atorB) {
        NoVertice verticeA= (this.vertices.getFim()), verticeB= null;
        NoAdjacente adjacenteA, adjacenteB;
                                                 // Caso o usuário tivesse acesso a inserção de arestas, usaria compareToIgnoreCase()
        if (atorA.compareTo(atorB)== 0) throw new IllegalArgumentException("Grafo deve ser simples!");

        // Busca otimizada: no pior caso, a lista de vértices é percorrida apenas uma vez para encontrar os dois vértices finais da aresta
        while((verticeA!= null) && (atorA.compareTo((String)verticeA.getAtor())!= 0) && (atorB.compareTo((String)verticeA.getAtor())!= 0))
            verticeA= verticeA.getAnterior();    // Busca do primeiro vértice final que receberá a aresta

        if (verticeA!= null) verticeB= verticeA.getAnterior();

        while((verticeB!= null) && (atorA.compareTo((String)verticeB.getAtor())!= 0) && (atorB.compareTo((String)verticeB.getAtor())!= 0))
            verticeB= verticeB.getAnterior();    // Busca do segundo vértice final que receberá a aresta

        if ((verticeA== null) || (verticeB== null)) throw new NoSuchElementException("Vértices não encontrados!");

        NoAresta auxiliar= ((NoAresta)this.arestas.insereNaLista(filme));
                                                 // Nova aresta é inserida na lista de arestas
        auxiliar.setVerticeA(verticeA);
        auxiliar.setVerticeB(verticeB);          // Ligação com os vértices finais

        adjacenteA= new NoAdjacente(auxiliar);   // Instanciação das respectivas estruturas de listas de adjacências
        adjacenteB= new NoAdjacente(auxiliar);

        adjacenteA.insereAdjacencia(verticeA);   // Ligação das estruturas de adjacências com os vértices finais e a aresta inserida
        adjacenteB.insereAdjacencia(verticeB);

        auxiliar.setAdjVerticeA(adjacenteA);     // Ligação da aresta com suas estruturas de listas de adjacências
        auxiliar.setAdjVerticeB(adjacenteB);
    }
    /**
     * Método remove a aresta referenciada e todas as suas estruturas de adjacências.
     * @param aresta Referência da aresta a ser removida
     * @throws NoSuchElementException no caso de não haver arestas a serem removidas
     */
    public void deletaAresta(NoAresta aresta) {
        NoAdjacente adjacenteA, adjacenteB;

        if (this.arestas.listaVazia()) throw new NoSuchElementException("Não há arestas no grafo!");
        else if (aresta== null) throw new NoSuchElementException("Aresta não encontrada!");
        else {
            adjacenteA= aresta.getAdjVerticeA();
            adjacenteB= aresta.getAdjVerticeB(); // Desligamento das respectivas estruturas de
                                                 // listas de adjacências
            adjacenteA.removeAdjacencia(aresta.getVerticeA());
            adjacenteB.removeAdjacencia(aresta.getVerticeB());

            aresta.setAdjVerticeA(null);    // Desligamento da aresta junto às suas respectivas
            aresta.setAdjVerticeB(null);    // estruturas de listas de adjacências

            this.arestas.removeDaLista(aresta);  // Remoção da aresta da lista de arestas
        }
    }
    /**
     * Retorna um inteiro com o número de elementos contidos na lista de arestas
     * do grafo.
     * @return Número de arestas contidas no grafo
     */
    public int numeroArestas() {
        return this.arestas.tamanhoLista();
    }
    /**
     * Método retorna uma aresta incidente a dois vértices.
     * @param adjacenteA Estrutura de adjacências referenciada pelo vértice final A
     * @param adjacenteB Estrutura de adjacências referenciada pelo vértice final B
     * @return Retorna a aresta incidente aos vértices, null caso não exista
     */
    public NoAresta retornaAresta(NoAdjacente adjacenteA, NoAdjacente adjacenteB) {
        NoAdjacente auxAdjacenteB;
        NoAresta arestaA, arestaB= null;

        while(adjacenteA!= null) {
            arestaA= adjacenteA.getAresta();
            auxAdjacenteB= adjacenteB;      // Busca nas listas de adjacências de dois
                                            // vértices até encontrar uma aresta
            while(auxAdjacenteB!= null) {   // incidente aos dois vértices
                arestaB= auxAdjacenteB.getAresta();

                if (arestaA== arestaB) return arestaB;     // Quando as arestas obtidas forem
                                                           // a mesma aresta, faz-se retorno
                auxAdjacenteB= auxAdjacenteB.getProximo();
            }
            adjacenteA= adjacenteA.getProximo();
        }
        return arestaB;
    }
    /**
     * Método recebe o nome de dois atores e então exibe o caminho mais curto
     * (formado por filmes) que leva o ator de origem até o ator de destino.
     * @param atorOrigem String com a identificação do vértice de origem
     * @param atorDestino String com a identificação do vértice de destino
     * @throws NullPointerException quando o arquivo não foi corretamente lido e,
     * portanto, não é possível calcular caminhos mínimos
     * @throws NoSuchElementException no caso de ator buscado não existir no grafo
     * @return Retorna uma String com o caminho mínimo encontrado
     */
    public String caminhoMinimo(String atorOrigem, String atorDestino) {
        NoVertice vertice= null;
        NoAresta aresta;
        NoAdjacente adjacenteA, adjacenteB;
        String caminho= "";

        try {
            vertice= dijkstra(atorOrigem, atorDestino);
        }
        catch (Exception erro) {
            throw new NullPointerException("Impossível realizar a operação solicitada!\n");
        }

        if (vertice== null) throw new NoSuchElementException("Este ator não existe!");
                                       // Caso em que o vértice de origem está em um componente conexo diferente do vértice de destino
        if (vertice.getDistancia()>= (Integer.MAX_VALUE/ 4)) caminho += ("\"" + vertice.getAtor() + "\" tem KB = infinito");
        else {
            caminho += ("\"" + vertice.getAtor() + "\" tem KB = " + vertice.getDistancia() + "\n\n");

            while(vertice.getPai()!= null) {
                adjacenteA= vertice.getListaAdjacencias();           // Caminho mais curto encontrado pelo
                adjacenteB= vertice.getPai().getListaAdjacencias();  // algoritmo de Dijkstra
                aresta= this.retornaAresta(adjacenteA, adjacenteB);

                caminho += ("\"" + vertice.getAtor() + "\" atuou em " + "\"" + aresta.getFilme() + "\" com " + "\"" + vertice.getPai().getAtor() + "\"\n");

                vertice= vertice.getPai();
            }
        }
        return caminho;
    }
    /**
     * Método implementa o algoritmo de Dijkstra para determinar caminhos mínimos.
     * Propriedade (Princípio de Otimalidade):
     * Um sub-caminho simples de um caminho mais curto simples é também um caminho
     * mais curto simples por si só.
     * Em outras palavras, se um vértice x faz parte do caminho mais curto de um
     * outro vértice u até um terceiro vértice v, então o sub-caminho entre
     * u e x é o caminho mais curto entre esses dois vértices.
     * @param atorOrigem String com a identificação do vértice de origem
     * @param atorDestino String com a identificação do vértice de destino
     * @return O vértice de destino encontrado, null caso não exista
     */
    public NoVertice dijkstra(String atorOrigem, String atorDestino) {
        int pesoAresta;
        NoVertice auxVertice1= this.vertices.getInicio(), auxVertice2, auxVertice3= null;
        NoAdjacente auxAdjacente;

        FilaDePrioridades fila= new FilaDePrioridades();     // Nova Fila de prioridades

        do {
            auxVertice1.setPai(null);       // Reserva-se a referência ao vértice de destino
            if ((atorDestino.compareToIgnoreCase((String)auxVertice1.getAtor()))== 0) auxVertice3= auxVertice1;
                                            // Vértice de origem recebe distância zero
            if ((atorOrigem.compareToIgnoreCase((String)auxVertice1.getAtor()))== 0) auxVertice1.setDistancia(0);
            else auxVertice1.setDistancia(Integer.MAX_VALUE/ 2);     // Demais vértices recebem valor "infinito" de distância

            fila.enfileira(auxVertice1);    // Todos os vértices são enfileirados
            auxVertice1= auxVertice1.getProximo();
        } while(auxVertice1!= null);

        while(!fila.filaVazia()) {
            auxVertice1= ((NoVertice)fila.desenfileira());   // Desenfileira o vértice de distância mínima presente na Fila
            auxAdjacente= auxVertice1.getListaAdjacencias();

            if (auxAdjacente!= null) {
                do {
                    auxVertice2= this.verticeOposto(auxVertice1, (auxAdjacente.getAresta()));
                    pesoAresta= auxAdjacente.getAresta().getPeso();

                    if ((auxVertice2.getDistancia())> (pesoAresta+ auxVertice1.getDistancia())) {
                        auxVertice2.setDistancia(pesoAresta+ auxVertice1.getDistancia());
                        auxVertice2.setPai(auxVertice1);     // Operação de atualização conhecida como "relaxamento", pois
                                                             // ela recebe a antiga estimativa e verifica se esta pode ser
                        fila.reorganizaFila(auxVertice2);    // melhorada para se aproximar de seu valor verdadeiro
                    }   // Reorganização da Fila de Prioridades após o processo de relaxamento
                    auxAdjacente= auxAdjacente.getProximo();

                } while(auxAdjacente!= null);
            }
        }
        return auxVertice3;       // Retorno com o vérice de destino, ou null caso ele não exista
    }
    /**
     * Método retorna a média das distâncias de todos os vértices até um vértice
     * específico, neste caso, o vértice de "Kevin Bacon".
     * O Princípio de Otimalidade implica que existe uma árvore de caminhos mais
     * curtos simples de um vértice inicial até todos os outros vértices. Ou seja,
     * não é necessário calcular a distância mínima n vezes dos n vértices para
     * obter cada uma das distâncias para o cálculo da média.
     * @param atorOrigemDestino String com o nome de um ator que servirá de origem
     * e destino ao mesmo tempo
     * @return Double com o valor da média das distâncias
     */
    public double getMedia(String atorOrigemDestino) {
        int totalVertices= 0;
        double media= 0;

        NoVertice auxVertice= this.vertices.getInicio();

        dijkstra(atorOrigemDestino, atorOrigemDestino);    // Algoritmo de Dijkstra

        while(auxVertice!= null) {

            if (auxVertice.getDistancia()< (Integer.MAX_VALUE/ 8)) {
                totalVertices++;                      // Vértices que estão em um componente conexo
                media += auxVertice.getDistancia();   // diferente e, portanto, têm distância "infinita",
            }                                         // não são considerado no cálculo da média
            auxVertice= auxVertice.getProximo();
        }
        media= (media/ totalVertices);

        return media;
    }
    /**
     * Método retorna o desvio padrão entre as distâncias de todos os vértices
     * até um vértice específico.
     * O Princípio de Otimalidade implica que existe uma árvore de caminhos mais
     * curtos simples de um vértice inicial até todos os outros vértices. Ou seja,
     * não é necessário calcular a distância mínima n vezes dos n vértices para
     * obter cada uma das distâncias para o cálculo do desvio padrão.
     * @param atorOrigemDestino String com o nome de um ator que servirá de origem
     * e destino ao mesmo tempo
     * @param media Valor da média das distâncias entre vértices
     * @return Double com o valor do desvio padrão entre as distâncias
     */
    public double getDesvioPadrao(String atorOrigemDestino, double media) {
        int totalVertices= 0;
        double desvio, somaQuadDesvios= 0, variancia, desvioPadrao;

        NoVertice auxVertice= this.vertices.getInicio();

        dijkstra(atorOrigemDestino, atorOrigemDestino);    // Algoritmo de Dijkstra

        while(auxVertice!= null) {

            if (auxVertice.getDistancia()< (Integer.MAX_VALUE/ 4)) {
                desvio= (auxVertice.getDistancia()- media);     // Desvio de cada distância
                somaQuadDesvios += Math.pow(desvio, 2);         // Soma dos quadrados dos desvios

                totalVertices++;                   // Vértices que estão em um componente conexo
            }                                      // diferente e, portanto, têm distância "infinita",
            auxVertice= auxVertice.getProximo();   // não são considerado no cálculo do desvio-parão
        }
        variancia= (somaQuadDesvios/ (totalVertices- 1));
        desvioPadrao= Math.sqrt(variancia);        // Desvio padrão: raiz quadrada da variância

        return desvioPadrao;
    }
    /**
     * Método que imprime o grafo em modo texto.
     * Primeiro são impressos os vértices e, em seguida, as arestas com seus respectivos
     * vértices incidentes. O método funciona, no entanto, para grafos muito grandes
     * sua execução pode ser um tanto demorada.
     */
    private void imprimeGrafo() {
        int tamanho= this.vertices.tamanhoLista();
        NoVertice auxVertice= this.vertices.getInicio();

        for (int i= 0; i< tamanho; i++) {   // Primeiro são impressos os vértices
            System.out.println(auxVertice.getAtor());

            auxVertice= auxVertice.getProximo();
        }
        System.out.println();

        tamanho= this.arestas.tamanhoLista();
        NoAresta auxAresta= this.arestas.getInicio();

        for (int i= 0; i< tamanho; i++) {   // Em seguida, são impressas as arestas com seus respectivos vértices incidentes
            System.out.println(auxAresta.getFilme() + " -- " + auxAresta.getVerticeA().getAtor() + " - " + auxAresta.getVerticeB().getAtor());

            auxAresta= auxAresta.getProximo();
        }
        System.out.println();
    }
    /**
     * Método responsável pela "desconstrução" do grafo, onde os vértices e suas
     * respectivas arestas incidentes são desligados de suas listas.
     */
    public void finalizaGrafo() {
        NoVertice auxiliar= this.vertices.getFim();

        while(auxiliar!= null) {
            this.deletaVertice((String)auxiliar.getAtor());
            auxiliar= this.vertices.getFim();    // Remoção de todos os vértices remove também
        }                                        // todas as respectivas arestas incidentes
        this.vertices= null;
        this.arestas= null;
    }
}
