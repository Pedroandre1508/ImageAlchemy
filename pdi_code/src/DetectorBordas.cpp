#include "DetectorBordas.hpp"
#include <cmath>
#include <iostream>

cv::Mat DetectorBordas::roberts(const cv::Mat& imagem) {
    // Converte para cinza se necessário
    cv::Mat imagemCinza = converterParaCinza(imagem);
    
    // Kernels de Roberts (2x2)
    // Gx detecta bordas verticais
    const int kernelGx[2][2] = {
        { 1,  0},
        { 0, -1}
    };
    
    // Gy detecta bordas horizontais
    const int kernelGy[2][2] = {
        { 0,  1},
        {-1,  0}
    };
    
    // Cria imagem de saída
    cv::Mat resultado = cv::Mat::zeros(imagemCinza.size(), CV_8UC1);
    
    // Aplica operador de Roberts
    for (int y = 0; y < imagemCinza.rows - 1; y++) {
        for (int x = 0; x < imagemCinza.cols - 1; x++) {
            // Calcula gradientes
            double gx = aplicarKernel2x2(imagemCinza, y, x, kernelGx);
            double gy = aplicarKernel2x2(imagemCinza, y, x, kernelGy);
            
            // Calcula magnitude
            double magnitude = calcularMagnitude(gx, gy);
            
            // Armazena resultado
            resultado.at<uchar>(y, x) = normalizar(magnitude);
        }
    }
    
    return resultado;
}

cv::Mat DetectorBordas::sobel(const cv::Mat& imagem) {
    // Converte para cinza se necessário
    cv::Mat imagemCinza = converterParaCinza(imagem);
    
    // Kernels de Sobel (3x3)
    // Gx detecta bordas verticais
    const int kernelGx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    
    // Gy detecta bordas horizontais
    const int kernelGy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };
    
    // Cria imagem de saída
    cv::Mat resultado = cv::Mat::zeros(imagemCinza.size(), CV_8UC1);
    
    // Aplica operador de Sobel
    for (int y = 1; y < imagemCinza.rows - 1; y++) {
        for (int x = 1; x < imagemCinza.cols - 1; x++) {
            // Calcula gradientes
            double gx = aplicarKernel3x3(imagemCinza, y, x, kernelGx);
            double gy = aplicarKernel3x3(imagemCinza, y, x, kernelGy);
            
            // Calcula magnitude
            double magnitude = calcularMagnitude(gx, gy);
            
            // Armazena resultado
            resultado.at<uchar>(y, x) = normalizar(magnitude);
        }
    }
    
    return resultado;
}

cv::Mat DetectorBordas::robinson(const cv::Mat& imagem) {
    // Converte para cinza se necessário
    cv::Mat imagemCinza = converterParaCinza(imagem);
    
    // Kernels de Robinson (8 direções)
    // Norte
    const int kernelN[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    
    // Nordeste
    const int kernelNE[3][3] = {
        { 0, 1, 2},
        {-1, 0, 1},
        {-2,-1, 0}
    };
    
    // Leste
    const int kernelE[3][3] = {
        { 1, 2, 1},
        { 0, 0, 0},
        {-1,-2,-1}
    };
    
    // Sudeste
    const int kernelSE[3][3] = {
        { 2, 1, 0},
        { 1, 0,-1},
        { 0,-1,-2}
    };
    
    // Sul
    const int kernelS[3][3] = {
        { 1, 0,-1},
        { 2, 0,-2},
        { 1, 0,-1}
    };
    
    // Sudoeste
    const int kernelSW[3][3] = {
        { 0,-1,-2},
        { 1, 0,-1},
        { 2, 1, 0}
    };
    
    // Oeste
    const int kernelW[3][3] = {
        {-1,-2,-1},
        { 0, 0, 0},
        { 1, 2, 1}
    };
    
    // Noroeste
    const int kernelNW[3][3] = {
        {-2,-1, 0},
        {-1, 0, 1},
        { 0, 1, 2}
    };
    
    // Cria imagem de saída
    cv::Mat resultado = cv::Mat::zeros(imagemCinza.size(), CV_8UC1);
    
    // Aplica operador de Robinson
    for (int y = 1; y < imagemCinza.rows - 1; y++) {
        for (int x = 1; x < imagemCinza.cols - 1; x++) {
            // Calcula resposta em todas as 8 direções
            double respostas[8];
            respostas[0] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelN));
            respostas[1] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelNE));
            respostas[2] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelE));
            respostas[3] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelSE));
            respostas[4] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelS));
            respostas[5] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelSW));
            respostas[6] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelW));
            respostas[7] = std::abs(aplicarKernel3x3(imagemCinza, y, x, kernelNW));
            
            // Pega o máximo de todas as direções
            double maxResposta = respostas[0];
            for (int i = 1; i < 8; i++) {
                if (respostas[i] > maxResposta) {
                    maxResposta = respostas[i];
                }
            }
            
            // Armazena resultado
            resultado.at<uchar>(y, x) = normalizar(maxResposta);
        }
    }
    
    return resultado;
}

cv::Mat DetectorBordas::aplicarLimiar(const cv::Mat& imagemBordas, int limiar) {
    cv::Mat resultado = imagemBordas.clone();
    
    for (int y = 0; y < imagemBordas.rows; y++) {
        for (int x = 0; x < imagemBordas.cols; x++) {
            uchar valor = imagemBordas.at<uchar>(y, x);
            resultado.at<uchar>(y, x) = (valor > limiar) ? 255 : 0;
        }
    }
    
    return resultado;
}

cv::Mat DetectorBordas::converterParaCinza(const cv::Mat& imagem) {
    if (imagem.channels() == 1) {
        return imagem.clone();
    }
    
    cv::Mat imagemCinza = cv::Mat(imagem.rows, imagem.cols, CV_8UC1);
    
    for (int y = 0; y < imagem.rows; y++) {
        for (int x = 0; x < imagem.cols; x++) {
            cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
            // Média ponderada para conversão RGB -> Cinza
            uchar cinza = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
            imagemCinza.at<uchar>(y, x) = cinza;
        }
    }
    
    return imagemCinza;
}

double DetectorBordas::calcularMagnitude(double gx, double gy) {
    // Magnitude = sqrt(Gx² + Gy²)
    return std::sqrt(gx * gx + gy * gy);
}

uchar DetectorBordas::normalizar(double valor) {
    // Normaliza para faixa 0-255
    if (valor > 255.0) {
        return 255;
    } else if (valor < 0.0) {
        return 0;
    } else {
        return static_cast<uchar>(valor);
    }
}

uchar DetectorBordas::tratarOverflow(double valor) {
    return normalizar(valor);
}

double DetectorBordas::aplicarKernel2x2(const cv::Mat& imagem, int y, int x, const int kernel[2][2]) {
    double soma = 0.0;
    
    for (int ky = 0; ky < 2; ky++) {
        for (int kx = 0; kx < 2; kx++) {
            uchar pixel = imagem.at<uchar>(y + ky, x + kx);
            soma += pixel * kernel[ky][kx];
        }
    }
    
    return soma;
}

double DetectorBordas::aplicarKernel3x3(const cv::Mat& imagem, int y, int x, const int kernel[3][3]) {
    double soma = 0.0;
    
    for (int ky = -1; ky <= 1; ky++) {
        for (int kx = -1; kx <= 1; kx++) {
            uchar pixel = imagem.at<uchar>(y + ky, x + kx);
            soma += pixel * kernel[ky + 1][kx + 1];
        }
    }
    
    return soma;
}
