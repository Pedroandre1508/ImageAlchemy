#include "OperacoesConvolucao.hpp"
#include <cmath>
#include <iostream>

cv::Mat OperacoesConvolucao::aplicarConvolucao(const cv::Mat& imagem, const cv::Mat& kernel) {
    // Valida o kernel
    if (!validarKernel(kernel)) {
        std::cerr << "Erro: Kernel inválido! Deve ser quadrado e ter dimensões ímpares." << std::endl;
        return imagem.clone();
    }
    
    // Converte para tons de cinza se necessário
    cv::Mat imagemCinza;
    if (imagem.channels() == 3) {
        imagemCinza = cv::Mat(imagem.rows, imagem.cols, CV_8UC1);
        for (int y = 0; y < imagem.rows; y++) {
            for (int x = 0; x < imagem.cols; x++) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                // Média ponderada para conversão RGB -> Cinza
                uchar cinza = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
                imagemCinza.at<uchar>(y, x) = cinza;
            }
        }
    } else {
        imagemCinza = imagem.clone();
    }
    
    // Cria imagem de saída
    cv::Mat resultado = cv::Mat::zeros(imagemCinza.size(), CV_8UC1);
    
    // Calcula o raio do kernel (distância do centro até a borda)
    int raio = kernel.rows / 2;
    
    // Aplica convolução pixel a pixel
    for (int y = raio; y < imagemCinza.rows - raio; y++) {
        for (int x = raio; x < imagemCinza.cols - raio; x++) {
            double soma = 0.0;
            
            // Aplica o kernel
            for (int ky = -raio; ky <= raio; ky++) {
                for (int kx = -raio; kx <= raio; kx++) {
                    // Pega o pixel da imagem
                    uchar pixel = imagemCinza.at<uchar>(y + ky, x + kx);
                    // Pega o valor correspondente do kernel
                    double kernelValor = kernel.at<double>(ky + raio, kx + raio);
                    // Multiplica e acumula
                    soma += pixel * kernelValor;
                }
            }
            
            // Trata overflow/underflow e armazena resultado
            resultado.at<uchar>(y, x) = tratarOverflow(soma);
        }
    }
    
    return resultado;
}

cv::Mat OperacoesConvolucao::criarKernelPassaAlta(int tamanho) {
    if (tamanho % 2 == 0) {
        std::cerr << "Erro: Tamanho do kernel deve ser ímpar!" << std::endl;
        tamanho = 3;
    }
    
    cv::Mat kernel = cv::Mat(tamanho, tamanho, CV_64F);
    int centro = tamanho / 2;
    
    // Preenche kernel com -1
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            kernel.at<double>(i, j) = -1.0;
        }
    }
    
    // Centro com valor positivo para criar passa-alta
    kernel.at<double>(centro, centro) = tamanho * tamanho;
    
    return kernel;
}

cv::Mat OperacoesConvolucao::criarKernelPassaBaixa(int tamanho) {
    if (tamanho % 2 == 0) {
        std::cerr << "Erro: Tamanho do kernel deve ser ímpar!" << std::endl;
        tamanho = 3;
    }
    
    // Kernel de média: todos os valores iguais
    cv::Mat kernel = cv::Mat(tamanho, tamanho, CV_64F);
    double valor = 1.0 / (tamanho * tamanho);
    
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            kernel.at<double>(i, j) = valor;
        }
    }
    
    return kernel;
}

cv::Mat OperacoesConvolucao::criarKernelNitidez(int tamanho) {
    if (tamanho % 2 == 0 || tamanho != 3) {
        std::cerr << "Aviso: Kernel de nitidez padrão usa tamanho 3x3" << std::endl;
        tamanho = 3;
    }
    
    // Kernel de nitidez 3x3 clássico
    cv::Mat kernel = cv::Mat(3, 3, CV_64F);
    kernel.at<double>(0, 0) = 0;   kernel.at<double>(0, 1) = -1;  kernel.at<double>(0, 2) = 0;
    kernel.at<double>(1, 0) = -1;  kernel.at<double>(1, 1) = 5;   kernel.at<double>(1, 2) = -1;
    kernel.at<double>(2, 0) = 0;   kernel.at<double>(2, 1) = -1;  kernel.at<double>(2, 2) = 0;
    
    return kernel;
}

bool OperacoesConvolucao::validarKernel(const cv::Mat& kernel) {
    // Verifica se é quadrado
    if (kernel.rows != kernel.cols) {
        return false;
    }
    
    // Verifica se as dimensões são ímpares
    if (kernel.rows % 2 == 0) {
        return false;
    }
    
    // Verifica se não está vazio
    if (kernel.empty()) {
        return false;
    }
    
    return true;
}

uchar OperacoesConvolucao::tratarOverflow(double valor) {
    if (valor > 255.0) {
        return 255;
    } else if (valor < 0.0) {
        return 0;
    } else {
        return static_cast<uchar>(valor);
    }
}
