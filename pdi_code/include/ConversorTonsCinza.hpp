#ifndef CONVERSOR_TONS_CINZA_HPP
#define CONVERSOR_TONS_CINZA_HPP

#include <opencv2/opencv.hpp>

class ConversorTonsCinza {
public:
    // Conversão usando média aritmética
    static cv::Mat paraMediaAritmetica(const cv::Mat& imagemColorida);
    
    // Conversão usando média ponderada
    static cv::Mat paraMediaPonderada(const cv::Mat& imagemColorida);
};

#endif // CONVERSOR_TONS_CINZA_HPP
