#include "OperacoesAritmeticas.hpp"
#include <algorithm>

cv::Mat OperacoesAritmeticas::somarEscalar(const cv::Mat& imagem, double valor) {
    cv::Mat resultado = imagem.clone();
    
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            if (imagem.channels() == 3) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int novoValor = pixel[c] + static_cast<int>(valor);
                    resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(novoValor);
                }
            } else if (imagem.channels() == 1) {
                int novoValor = imagem.at<uchar>(y, x) + static_cast<int>(valor);
                resultado.at<uchar>(y, x) = cv::saturate_cast<uchar>(novoValor);
            }
        }
    }
    return resultado;
}

cv::Mat OperacoesAritmeticas::subtrairEscalar(const cv::Mat& imagem, double valor) {
    cv::Mat resultado = imagem.clone();
    
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            if (imagem.channels() == 3) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int novoValor = pixel[c] - static_cast<int>(valor);
                    resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(novoValor);
                }
            } else if (imagem.channels() == 1) {
                int novoValor = imagem.at<uchar>(y, x) - static_cast<int>(valor);
                resultado.at<uchar>(y, x) = cv::saturate_cast<uchar>(novoValor);
            }
        }
    }
    return resultado;
}

cv::Mat OperacoesAritmeticas::multiplicarEscalar(const cv::Mat& imagem, double valor) {
    cv::Mat resultado = imagem.clone();
    
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            if (imagem.channels() == 3) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    double novoValor = pixel[c] * valor;
                    resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(novoValor);
                }
            } else if (imagem.channels() == 1) {
                double novoValor = imagem.at<uchar>(y, x) * valor;
                resultado.at<uchar>(y, x) = cv::saturate_cast<uchar>(novoValor);
            }
        }
    }
    return resultado;
}

cv::Mat OperacoesAritmeticas::dividirEscalar(const cv::Mat& imagem, double valor) {
    cv::Mat resultado = imagem.clone();
    
    if (valor == 0) {
        std::cerr << "Erro: Divisão por zero!" << std::endl;
        return resultado;
    }
    
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            if (imagem.channels() == 3) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    double novoValor = pixel[c] / valor;
                    resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(novoValor);
                }
            } else if (imagem.channels() == 1) {
                double novoValor = imagem.at<uchar>(y, x) / valor;
                resultado.at<uchar>(y, x) = cv::saturate_cast<uchar>(novoValor);
            }
        }
    }
    return resultado;
}

cv::Mat OperacoesAritmeticas::somarImagens(const cv::Mat& img1, const cv::Mat& img2) {
    // Garantir que as imagens tenham o mesmo tamanho
    int altura = std::min(img1.rows, img2.rows);
    int largura = std::min(img1.cols, img2.cols);
    cv::Mat resultado(altura, largura, img1.type());
    
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int soma = pixel1[c] + pixel2[c];
                    resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(soma);
                }
            }
        }
    }
    return resultado;
}

cv::Mat OperacoesAritmeticas::subtrairImagens(const cv::Mat& img1, const cv::Mat& img2) {
    // Garantir que as imagens tenham o mesmo tamanho
    int altura = std::min(img1.rows, img2.rows);
    int largura = std::min(img1.cols, img2.cols);
    cv::Mat resultado(altura, largura, img1.type());
    
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int diferenca = pixel1[c] - pixel2[c];
                    resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(diferenca);
                }
            }
        }
    }
    return resultado;
}

cv::Mat OperacoesAritmeticas::multiplicarImagens(const cv::Mat& img1, const cv::Mat& img2) {
    // Garantir que as imagens tenham o mesmo tamanho
    int altura = std::min(img1.rows, img2.rows);
    int largura = std::min(img1.cols, img2.cols);
    cv::Mat resultado(altura, largura, img1.type());
    
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int produto = (pixel1[c] * pixel2[c]) / 255;
                    resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(produto);
                }
            }
        }
    }
    return resultado;
}

cv::Mat OperacoesAritmeticas::dividirImagens(const cv::Mat& img1, const cv::Mat& img2) {
    // Garantir que as imagens tenham o mesmo tamanho
    int altura = std::min(img1.rows, img2.rows);
    int largura = std::min(img1.cols, img2.cols);
    cv::Mat resultado(altura, largura, img1.type());
    
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    if (pixel2[c] == 0) {
                        resultado.at<cv::Vec3b>(y, x)[c] = 255;
                    } else {
                        int divisao = (pixel1[c] * 255) / pixel2[c];
                        resultado.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(divisao);
                    }
                }
            }
        }
    }
    return resultado;
}
