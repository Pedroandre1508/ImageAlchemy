#ifndef PROCESSADOR_HISTOGRAMAS_HPP
#define PROCESSADOR_HISTOGRAMAS_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class ProcessadorHistogramas {
public:
    static std::vector<cv::Mat> calcularHistograma(const cv::Mat& imagem);
    static cv::Mat visualizarHistograma(const std::vector<cv::Mat>& histogramas);
    static cv::Mat equalizarHistograma(const cv::Mat& imagem);
};

#endif
