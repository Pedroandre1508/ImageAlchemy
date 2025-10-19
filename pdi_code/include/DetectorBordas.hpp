#ifndef DETECTOR_BORDAS_HPP
#define DETECTOR_BORDAS_HPP

#include <opencv2/opencv.hpp>

/**
 * CLASSE: DetectorBordas
 * 
 * Implementa algoritmos de detecção de bordas em imagens em tons de cinza.
 * Operadores disponíveis: Roberts, Sobel e Robinson.
 * 
 * Restrições:
 * - Aplica-se apenas em imagens em tons de cinza
 * - Implementação manual pixel a pixel (sem usar funções do OpenCV)
 * 
 * Conceitos:
 * - Roberts: Operador 2x2 que detecta bordas diagonais (rápido mas sensível a ruído)
 * - Sobel: Operador 3x3 que detecta bordas horizontais e verticais (mais robusto)
 * - Robinson: Operador 3x3 direcional que detecta bordas em 8 direções
 * 
 * Todos os métodos retornam a magnitude do gradiente (combinação de Gx e Gy)
 */
class DetectorBordas {
public:
    /**
     * Detecta bordas usando operador de Roberts
     * Usa dois kernels 2x2 para detectar gradientes diagonais
     * @param imagem Imagem em tons de cinza
     * @return Imagem com bordas detectadas
     */
    static cv::Mat roberts(const cv::Mat& imagem);
    
    /**
     * Detecta bordas usando operador de Sobel
     * Usa dois kernels 3x3 para detectar gradientes horizontal e vertical
     * @param imagem Imagem em tons de cinza
     * @return Imagem com bordas detectadas
     */
    static cv::Mat sobel(const cv::Mat& imagem);
    
    /**
     * Detecta bordas usando operador de Robinson
     * Usa 8 kernels 3x3 para detectar bordas em todas as direções
     * @param imagem Imagem em tons de cinza
     * @return Imagem com bordas detectadas
     */
    static cv::Mat robinson(const cv::Mat& imagem);
    
    /**
     * Aplica limiarização no resultado para destacar bordas fortes
     * @param imagemBordas Imagem com bordas detectadas
     * @param limiar Valor de limiar (0-255)
     * @return Imagem binária com bordas
     */
    static cv::Mat aplicarLimiar(const cv::Mat& imagemBordas, int limiar = 50);

private:
    /**
     * Converte imagem colorida para tons de cinza se necessário
     */
    static cv::Mat converterParaCinza(const cv::Mat& imagem);
    
    /**
     * Calcula magnitude do gradiente a partir de Gx e Gy
     * Magnitude = sqrt(Gx² + Gy²)
     */
    static double calcularMagnitude(double gx, double gy);
    
    /**
     * Normaliza valores do gradiente para faixa 0-255
     */
    static uchar normalizar(double valor);
    
    /**
     * Trata overflow e underflow
     */
    static uchar tratarOverflow(double valor);
    
    /**
     * Aplica kernel 2x2 (para Roberts)
     */
    static double aplicarKernel2x2(const cv::Mat& imagem, int y, int x, const int kernel[2][2]);
    
    /**
     * Aplica kernel 3x3 (para Sobel e Robinson)
     */
    static double aplicarKernel3x3(const cv::Mat& imagem, int y, int x, const int kernel[3][3]);
};

#endif
