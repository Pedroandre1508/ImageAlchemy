#include "ProcessadorImagens.hpp"

cv::Mat ProcessadorImagens::aplicarLimiarizacao(const cv::Mat& imagem, double limiar, double valorMaximo) {
    cv::Mat resultado = imagem.clone();
    
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            if (imagem.channels() == 3) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                // Converter para cinza usando média ponderada
                uchar cinza = static_cast<uchar>(0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
                uchar valor = (cinza > limiar) ? static_cast<uchar>(valorMaximo) : 0;
                resultado.at<cv::Vec3b>(y, x) = cv::Vec3b(valor, valor, valor);
            } else if (imagem.channels() == 1) {
                uchar pixel = imagem.at<uchar>(y, x);
                resultado.at<uchar>(y, x) = (pixel > limiar) ? static_cast<uchar>(valorMaximo) : 0;
            }
        }
    }
    return resultado;
}

cv::Mat ProcessadorImagens::isolarCanal(const cv::Mat& imagem, int canal) {
    cv::Mat resultado = cv::Mat::zeros(imagem.size(), imagem.type());
    
    if (imagem.channels() == 3 && canal >= 0 && canal <= 2) {
        for (int y = 0; y < imagem.rows; y++) {
            for (int x = 0; x < imagem.cols; x++) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                cv::Vec3b novoPixel(0, 0, 0);
                novoPixel[canal] = pixel[canal];
                resultado.at<cv::Vec3b>(y, x) = novoPixel;
            }
        }
    }
    return resultado;
}

cv::Mat ProcessadorImagens::inverterImagem(const cv::Mat& imagem) {
    cv::Mat resultado = imagem.clone();
    
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            if (imagem.channels() == 3) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                resultado.at<cv::Vec3b>(y, x) = cv::Vec3b(255 - pixel[0], 255 - pixel[1], 255 - pixel[2]);
            } else if (imagem.channels() == 1) {
                resultado.at<uchar>(y, x) = 255 - imagem.at<uchar>(y, x);
            }
        }
    }
    return resultado;
}
