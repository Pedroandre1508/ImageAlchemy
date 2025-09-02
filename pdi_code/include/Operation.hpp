#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class Operation 
{
public:
    Operation();
    
    // Conversão para tons de cinza
    static cv::Mat toGrayscaleAverage(const cv::Mat& input);
    static cv::Mat toGrayscaleWeighted(const cv::Mat& input);
    
    // Conversão para tons de cinza real (1 canal)
    static cv::Mat toGrayscaleRealChannel(const cv::Mat& input);
    
    // Operações aritméticas
    static cv::Mat addImages(const cv::Mat& img1, const cv::Mat& img2);
    static cv::Mat subtractImages(const cv::Mat& img1, const cv::Mat& img2);
    static cv::Mat multiplyImages(const cv::Mat& img1, const cv::Mat& img2);
    static cv::Mat divideImages(const cv::Mat& img1, const cv::Mat& img2);
    
    static cv::Mat addScalar(const cv::Mat& input, double scalar);
    static cv::Mat subtractScalar(const cv::Mat& input, double scalar);
    static cv::Mat multiplyScalar(const cv::Mat& input, double scalar);
    static cv::Mat divideScalar(const cv::Mat& input, double scalar);
    
    // Limiarização
    static cv::Mat threshold(const cv::Mat& input, double thresh, double maxval);
    
    // Isolamento de canais
    static cv::Mat isolateChannel(const cv::Mat& input, int channel); // 0=B, 1=G, 2=R
    
    // Histograma
    static std::vector<cv::Mat> computeHistogram(const cv::Mat& input);
    static cv::Mat visualizeHistogram(const std::vector<cv::Mat>& histograms);
    
    // Inverso da imagem
    static cv::Mat invert(const cv::Mat& input);
    
    // Método específico para teste de histograma
    static void testHistogramImages();

private:
    
};

#endif // OPERATION_HPP
