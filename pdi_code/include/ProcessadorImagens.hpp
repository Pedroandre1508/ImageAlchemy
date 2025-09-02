#ifndef PROCESSADOR_IMAGENS_HPP
#define PROCESSADOR_IMAGENS_HPP

#include <opencv2/opencv.hpp>

class ProcessadorImagens {
public:
    static cv::Mat aplicarLimiarizacao(const cv::Mat& imagem, double limiar, double valorMaximo = 255);
    static cv::Mat isolarCanal(const cv::Mat& imagem, int canal);
    static cv::Mat inverterImagem(const cv::Mat& imagem);
};

#endif
