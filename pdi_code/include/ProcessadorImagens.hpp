#ifndef PROCESSADOR_IMAGENS_HPP
#define PROCESSADOR_IMAGENS_HPP

#include <opencv2/opencv.hpp>

class ProcessadorImagens {
public:
    // Limiarização
    static cv::Mat aplicarLimiarizacao(const cv::Mat& imagem, double limiar, double valorMaximo);
    
    // Isolamento de canais (0=Azul, 1=Verde, 2=Vermelho)
    static cv::Mat isolarCanal(const cv::Mat& imagem, int canal);
    
    // Inverso da imagem
    static cv::Mat inverterImagem(const cv::Mat& imagem);
};

#endif // PROCESSADOR_IMAGENS_HPP
