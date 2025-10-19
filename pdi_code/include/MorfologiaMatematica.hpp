#ifndef MORFOLOGIA_MATEMATICA_HPP
#define MORFOLOGIA_MATEMATICA_HPP

#include <opencv2/opencv.hpp>

/**
 * CLASSE: MorfologiaMatematica
 * 
 * Implementa operações de morfologia matemática em imagens binárias.
 * Operações disponíveis: Erosão, Dilatação, Abertura, Fechamento e Limites.
 * 
 * Restrições:
 * - Aplica-se apenas em imagens binárias (preto e branco)
 * - Elemento estruturante deve ter dimensões ímpares
 * - Implementação manual pixel a pixel (sem usar funções do OpenCV)
 * 
 * Conceitos:
 * - Erosão: Remove pixels da borda dos objetos (encolhe)
 * - Dilatação: Adiciona pixels na borda dos objetos (expande)
 * - Abertura: Erosão seguida de Dilatação (remove ruídos pequenos)
 * - Fechamento: Dilatação seguida de Erosão (preenche buracos pequenos)
 * - Limites: Extrai as bordas dos objetos
 */
class MorfologiaMatematica {
public:
    /**
     * Aplica erosão na imagem binária
     * @param imagem Imagem binária (deve ser limiarizada)
     * @param elementoEstruturante Elemento estruturante (kernel)
     * @return Imagem erodida
     */
    static cv::Mat erosao(const cv::Mat& imagem, const cv::Mat& elementoEstruturante);
    
    /**
     * Aplica dilatação na imagem binária
     * @param imagem Imagem binária (deve ser limiarizada)
     * @param elementoEstruturante Elemento estruturante (kernel)
     * @return Imagem dilatada
     */
    static cv::Mat dilatacao(const cv::Mat& imagem, const cv::Mat& elementoEstruturante);
    
    /**
     * Aplica abertura (erosão + dilatação)
     * Remove ruídos pequenos mantendo a forma dos objetos
     * @param imagem Imagem binária
     * @param elementoEstruturante Elemento estruturante
     * @return Imagem após abertura
     */
    static cv::Mat abertura(const cv::Mat& imagem, const cv::Mat& elementoEstruturante);
    
    /**
     * Aplica fechamento (dilatação + erosão)
     * Preenche buracos pequenos mantendo a forma dos objetos
     * @param imagem Imagem binária
     * @param elementoEstruturante Elemento estruturante
     * @return Imagem após fechamento
     */
    static cv::Mat fechamento(const cv::Mat& imagem, const cv::Mat& elementoEstruturante);
    
    /**
     * Extrai limite interno (borda interna dos objetos)
     * Limite Interno = Original - Erosão
     * @param imagem Imagem binária
     * @param elementoEstruturante Elemento estruturante
     * @return Imagem com limite interno
     */
    static cv::Mat limiteInterno(const cv::Mat& imagem, const cv::Mat& elementoEstruturante);
    
    /**
     * Extrai limite externo (borda externa dos objetos)
     * Limite Externo = Dilatação - Original
     * @param imagem Imagem binária
     * @param elementoEstruturante Elemento estruturante
     * @return Imagem com limite externo
     */
    static cv::Mat limiteExterno(const cv::Mat& imagem, const cv::Mat& elementoEstruturante);
    
    /**
     * Cria elemento estruturante quadrado
     * @param tamanho Tamanho do elemento (deve ser ímpar)
     * @return Elemento estruturante quadrado (todos os valores = 1)
     */
    static cv::Mat criarElementoEstruturante(int tamanho = 3);
    
    /**
     * Cria elemento estruturante em forma de cruz
     * @param tamanho Tamanho do elemento (deve ser ímpar)
     * @return Elemento estruturante em cruz
     */
    static cv::Mat criarElementoEstruturanteCruz(int tamanho = 3);
    
    /**
     * Converte imagem para binária (limiarização)
     * @param imagem Imagem em tons de cinza
     * @param limiar Valor de limiar (0-255)
     * @return Imagem binária
     */
    static cv::Mat converterParaBinaria(const cv::Mat& imagem, int limiar = 128);

private:
    /**
     * Verifica se elemento estruturante encaixa completamente no pixel
     * (usado na erosão)
     */
    static bool encaixaCompletamente(const cv::Mat& imagem, int y, int x, const cv::Mat& ee);
    
    /**
     * Verifica se elemento estruturante tem alguma intersecção com o pixel
     * (usado na dilatação)
     */
    static bool temIntersecao(const cv::Mat& imagem, int y, int x, const cv::Mat& ee);
};

#endif
