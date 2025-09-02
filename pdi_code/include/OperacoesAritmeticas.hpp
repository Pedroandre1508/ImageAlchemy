#ifndef OPERACOES_ARITMETICAS_HPP
#define OPERACOES_ARITMETICAS_HPP

#include <opencv2/opencv.hpp>
#include <iostream>

class OperacoesAritmeticas {
public:
    static cv::Mat somarEscalar(const cv::Mat& imagem, double valor);
    static cv::Mat subtrairEscalar(const cv::Mat& imagem, double valor);
    static cv::Mat multiplicarEscalar(const cv::Mat& imagem, double valor);
    static cv::Mat dividirEscalar(const cv::Mat& imagem, double valor);
    
    static cv::Mat somarImagens(const cv::Mat& img1, const cv::Mat& img2);
    static cv::Mat subtrairImagens(const cv::Mat& img1, const cv::Mat& img2);
    static cv::Mat multiplicarImagens(const cv::Mat& img1, const cv::Mat& img2);
    static cv::Mat dividirImagens(const cv::Mat& img1, const cv::Mat& img2);
};

#endif
