#ifndef OPERACOES_CONVOLUCAO_HPP
#define OPERACOES_CONVOLUCAO_HPP

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * CLASSE: OperacoesConvolucao
 * 
 * Implementa operações de convolução em imagens em tons de cinza.
 * Permite aplicar diferentes kernels (filtros) de tamanho ímpar com hot spot no centro.
 * 
 * Restrições:
 * - Kernel deve ter dimensões ímpares (3x3, 5x5, 7x7, etc)
 * - Hot spot sempre no centro do kernel
 * - Aplica-se apenas em imagens em tons de cinza
 * 
 * Implementação manual pixel a pixel (sem usar funções de convolução do OpenCV)
 */
class OperacoesConvolucao {
public:
    /**
     * Aplica convolução com kernel personalizado
     * @param imagem Imagem em tons de cinza (1 canal)
     * @param kernel Matriz do kernel (deve ser quadrada e ímpar)
     * @return Imagem resultante após convolução
     */
    static cv::Mat aplicarConvolucao(const cv::Mat& imagem, const cv::Mat& kernel);
    
    /**
     * Cria kernel para detecção de bordas (passa-alta)
     * @param tamanho Tamanho do kernel (deve ser ímpar)
     * @return Kernel de detecção de bordas
     */
    static cv::Mat criarKernelPassaAlta(int tamanho = 3);
    
    /**
     * Cria kernel para suavização (passa-baixa / blur)
     * @param tamanho Tamanho do kernel (deve ser ímpar)
     * @return Kernel de suavização
     */
    static cv::Mat criarKernelPassaBaixa(int tamanho = 3);
    
    /**
     * Cria kernel para nitidez (sharpening)
     * @param tamanho Tamanho do kernel (deve ser ímpar)
     * @return Kernel de nitidez
     */
    static cv::Mat criarKernelNitidez(int tamanho = 3);
    
    /**
     * Valida se o kernel é válido (quadrado e ímpar)
     * @param kernel Kernel a ser validado
     * @return true se válido, false caso contrário
     */
    static bool validarKernel(const cv::Mat& kernel);
    
private:
    /**
     * Trata valores fora dos limites [0, 255] (overflow e underflow)
     * @param valor Valor a ser tratado
     * @return Valor limitado entre 0 e 255
     */
    static uchar tratarOverflow(double valor);
};

#endif
