#include "ConversorTonsCinza.hpp"

cv::Mat ConversorTonsCinza::paraMediaAritmetica(const cv::Mat& imagemColorida) {
    cv::Mat resultado = imagemColorida.clone();
    
    for (int y = 0; y < imagemColorida.rows; y++) {
        for (int x = 0; x < imagemColorida.cols; x++) {
            if (imagemColorida.channels() == 3) {
                cv::Vec3b pixel = imagemColorida.at<cv::Vec3b>(y, x);
                uchar media = (pixel[0] + pixel[1] + pixel[2]) / 3;
                resultado.at<cv::Vec3b>(y, x) = cv::Vec3b(media, media, media);
            }
        }
    }
    return resultado;
}

cv::Mat ConversorTonsCinza::paraMediaPonderada(const cv::Mat& imagemColorida) {
    cv::Mat resultado = imagemColorida.clone();
    
    for (int y = 0; y < imagemColorida.rows; y++) {
        for (int x = 0; x < imagemColorida.cols; x++) {
            if (imagemColorida.channels() == 3) {
                cv::Vec3b pixel = imagemColorida.at<cv::Vec3b>(y, x);
                // Fórmula padrão ITU-R BT.709: 0.299*R + 0.587*G + 0.114*B
                uchar mediaPonderada = static_cast<uchar>(0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
                resultado.at<cv::Vec3b>(y, x) = cv::Vec3b(mediaPonderada, mediaPonderada, mediaPonderada);
            }
        }
    }
    return resultado;
}
