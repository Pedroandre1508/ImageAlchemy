#ifndef PROCESSADOR_HISTOGRAMAS_HPP
#define PROCESSADOR_HISTOGRAMAS_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class ProcessadorHistogramas {
public:
    // Calcular histograma
    static std::vector<cv::Mat> calcularHistograma(const cv::Mat& imagem);
    
    // Visualizar histograma
    static cv::Mat visualizarHistograma(const std::vector<cv::Mat>& histogramas);
    
    // Equalização de histograma
    static cv::Mat equalizarHistograma(const cv::Mat& imagem);
};

#endif // PROCESSADOR_HISTOGRAMAS_HPP
