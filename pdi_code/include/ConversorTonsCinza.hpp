#ifndef CONVERSOR_TONS_CINZA_HPP
#define CONVERSOR_TONS_CINZA_HPP

#include <opencv2/opencv.hpp>

class ConversorTonsCinza {
public:
    static cv::Mat paraMediaAritmetica(const cv::Mat& imagemColorida);
    static cv::Mat paraMediaPonderada(const cv::Mat& imagemColorida);
};

#endif
