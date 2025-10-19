#include "MorfologiaMatematica.hpp"
#include <iostream>

cv::Mat MorfologiaMatematica::erosao(const cv::Mat& imagem, const cv::Mat& elementoEstruturante) {
    // Converte para binária se necessário
    cv::Mat imagemBinaria = converterParaBinaria(imagem);
    
    // Cria imagem de saída
    cv::Mat resultado = cv::Mat::zeros(imagemBinaria.size(), CV_8UC1);
    
    // Calcula raio do elemento estruturante
    int raio = elementoEstruturante.rows / 2;
    
    // Aplica erosão
    for (int y = raio; y < imagemBinaria.rows - raio; y++) {
        for (int x = raio; x < imagemBinaria.cols - raio; x++) {
            // Se o elemento estruturante encaixa completamente, mantém o pixel
            if (encaixaCompletamente(imagemBinaria, y, x, elementoEstruturante)) {
                resultado.at<uchar>(y, x) = 255;
            } else {
                resultado.at<uchar>(y, x) = 0;
            }
        }
    }
    
    return resultado;
}

cv::Mat MorfologiaMatematica::dilatacao(const cv::Mat& imagem, const cv::Mat& elementoEstruturante) {
    // Converte para binária se necessário
    cv::Mat imagemBinaria = converterParaBinaria(imagem);
    
    // Cria imagem de saída
    cv::Mat resultado = cv::Mat::zeros(imagemBinaria.size(), CV_8UC1);
    
    // Calcula raio do elemento estruturante
    int raio = elementoEstruturante.rows / 2;
    
    // Aplica dilatação
    for (int y = raio; y < imagemBinaria.rows - raio; y++) {
        for (int x = raio; x < imagemBinaria.cols - raio; x++) {
            // Se há alguma intersecção, ativa o pixel
            if (temIntersecao(imagemBinaria, y, x, elementoEstruturante)) {
                resultado.at<uchar>(y, x) = 255;
            } else {
                resultado.at<uchar>(y, x) = 0;
            }
        }
    }
    
    return resultado;
}

cv::Mat MorfologiaMatematica::abertura(const cv::Mat& imagem, const cv::Mat& elementoEstruturante) {
    // Abertura = Erosão seguida de Dilatação
    cv::Mat erodida = erosao(imagem, elementoEstruturante);
    cv::Mat resultado = dilatacao(erodida, elementoEstruturante);
    return resultado;
}

cv::Mat MorfologiaMatematica::fechamento(const cv::Mat& imagem, const cv::Mat& elementoEstruturante) {
    // Fechamento = Dilatação seguida de Erosão
    cv::Mat dilatada = dilatacao(imagem, elementoEstruturante);
    cv::Mat resultado = erosao(dilatada, elementoEstruturante);
    return resultado;
}

cv::Mat MorfologiaMatematica::limiteInterno(const cv::Mat& imagem, const cv::Mat& elementoEstruturante) {
    // Limite Interno = Original - Erosão
    cv::Mat imagemBinaria = converterParaBinaria(imagem);
    cv::Mat erodida = erosao(imagemBinaria, elementoEstruturante);
    
    cv::Mat resultado = cv::Mat::zeros(imagemBinaria.size(), CV_8UC1);
    
    // Subtração pixel a pixel
    for (int y = 0; y < imagemBinaria.rows; y++) {
        for (int x = 0; x < imagemBinaria.cols; x++) {
            uchar original = imagemBinaria.at<uchar>(y, x);
            uchar erodido = erodida.at<uchar>(y, x);
            
            // Se estava ativo no original mas não na erodida, é borda interna
            if (original == 255 && erodido == 0) {
                resultado.at<uchar>(y, x) = 255;
            }
        }
    }
    
    return resultado;
}

cv::Mat MorfologiaMatematica::limiteExterno(const cv::Mat& imagem, const cv::Mat& elementoEstruturante) {
    // Limite Externo = Dilatação - Original
    cv::Mat imagemBinaria = converterParaBinaria(imagem);
    cv::Mat dilatada = dilatacao(imagemBinaria, elementoEstruturante);
    
    cv::Mat resultado = cv::Mat::zeros(imagemBinaria.size(), CV_8UC1);
    
    // Subtração pixel a pixel
    for (int y = 0; y < imagemBinaria.rows; y++) {
        for (int x = 0; x < imagemBinaria.cols; x++) {
            uchar dilatado = dilatada.at<uchar>(y, x);
            uchar original = imagemBinaria.at<uchar>(y, x);
            
            // Se está ativo na dilatada mas não no original, é borda externa
            if (dilatado == 255 && original == 0) {
                resultado.at<uchar>(y, x) = 255;
            }
        }
    }
    
    return resultado;
}

cv::Mat MorfologiaMatematica::criarElementoEstruturante(int tamanho) {
    if (tamanho % 2 == 0) {
        std::cerr << "Erro: Tamanho do elemento estruturante deve ser ímpar!" << std::endl;
        tamanho = 3;
    }
    
    // Cria elemento estruturante quadrado (todos os valores = 1)
    cv::Mat elemento = cv::Mat::ones(tamanho, tamanho, CV_8UC1);
    return elemento;
}

cv::Mat MorfologiaMatematica::criarElementoEstruturanteCruz(int tamanho) {
    if (tamanho % 2 == 0 || tamanho < 3) {
        std::cerr << "Erro: Tamanho do elemento estruturante deve ser ímpar e >= 3!" << std::endl;
        tamanho = 3;
    }
    
    // Cria elemento estruturante em forma de cruz
    cv::Mat elemento = cv::Mat::zeros(tamanho, tamanho, CV_8UC1);
    int centro = tamanho / 2;
    
    // Linha horizontal
    for (int x = 0; x < tamanho; x++) {
        elemento.at<uchar>(centro, x) = 1;
    }
    
    // Linha vertical
    for (int y = 0; y < tamanho; y++) {
        elemento.at<uchar>(y, centro) = 1;
    }
    
    return elemento;
}

cv::Mat MorfologiaMatematica::converterParaBinaria(const cv::Mat& imagem, int limiar) {
    cv::Mat resultado;
    
    // Se já é binária, retorna clone
    if (imagem.channels() == 1) {
        resultado = imagem.clone();
    } else {
        // Converte para cinza primeiro
        resultado = cv::Mat(imagem.rows, imagem.cols, CV_8UC1);
        for (int y = 0; y < imagem.rows; y++) {
            for (int x = 0; x < imagem.cols; x++) {
                cv::Vec3b pixel = imagem.at<cv::Vec3b>(y, x);
                uchar cinza = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
                resultado.at<uchar>(y, x) = cinza;
            }
        }
    }
    
    // Aplica limiarização para garantir imagem binária
    for (int y = 0; y < resultado.rows; y++) {
        for (int x = 0; x < resultado.cols; x++) {
            uchar pixel = resultado.at<uchar>(y, x);
            resultado.at<uchar>(y, x) = (pixel > limiar) ? 255 : 0;
        }
    }
    
    return resultado;
}

bool MorfologiaMatematica::encaixaCompletamente(const cv::Mat& imagem, int y, int x, const cv::Mat& ee) {
    int raio = ee.rows / 2;
    
    // Verifica se todos os pixels cobertos pelo EE são brancos
    for (int ky = -raio; ky <= raio; ky++) {
        for (int kx = -raio; kx <= raio; kx++) {
            // Verifica se o elemento estruturante tem valor 1 nesta posição
            if (ee.at<uchar>(ky + raio, kx + raio) == 1) {
                // Verifica se o pixel correspondente na imagem é branco
                if (imagem.at<uchar>(y + ky, x + kx) != 255) {
                    return false; // Não encaixa completamente
                }
            }
        }
    }
    
    return true; // Encaixa completamente
}

bool MorfologiaMatematica::temIntersecao(const cv::Mat& imagem, int y, int x, const cv::Mat& ee) {
    int raio = ee.rows / 2;
    
    // Verifica se algum pixel coberto pelo EE é branco
    for (int ky = -raio; ky <= raio; ky++) {
        for (int kx = -raio; kx <= raio; kx++) {
            // Verifica se o elemento estruturante tem valor 1 nesta posição
            if (ee.at<uchar>(ky + raio, kx + raio) == 1) {
                // Verifica se o pixel correspondente na imagem é branco
                if (imagem.at<uchar>(y + ky, x + kx) == 255) {
                    return true; // Tem intersecção
                }
            }
        }
    }
    
    return false; // Não tem intersecção
}
