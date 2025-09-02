#include "Operation.hpp"
#include <algorithm>
#include <iostream>

Operation::Operation() {}

// Função auxiliar para saturação manual com documentação explícita
inline uchar saturateValue(int value) {
    // TRATAMENTO DE OVERFLOW/UNDERFLOW:
    // - Valores < 0 (underflow) → 0
    // - Valores > 255 (overflow) → 255
    // - Valores válidos [0,255] → mantém valor original
    if (value < 0) return 0;        // Tratamento de underflow
    if (value > 255) return 255;    // Tratamento de overflow
    return static_cast<uchar>(value);
}

// Função auxiliar para arredondamento manual (substitui cvRound)
inline int roundToInt(double value) {
    return static_cast<int>(value + 0.5);
}

/**
 * CONVERSÃO PARA TONS DE CINZA - MÉDIA ARITMÉTICA
 * 
 * Funcionamento:
 * - Para cada pixel da imagem colorida (BGR), calcula a média simples dos 3 canais
 * - Fórmula: Cinza = (B + G + R) / 3
 * - Aplica o mesmo valor calculado nos 3 canais para manter compatibilidade com imagens coloridas
 * 
 * Vantagens: Simples e rápido
 * Desvantagens: Não considera a percepção visual humana (olho é mais sensível ao verde)
 */
cv::Mat Operation::toGrayscaleAverage(const cv::Mat& input) {
    cv::Mat output = input.clone();  // Cria uma cópia da imagem original
    
    // Percorre cada pixel da imagem
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            if (input.channels() == 3) {  // Verifica se é imagem colorida (3 canais)
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);  // Lê o pixel BGR
                
                // Calcula a média aritmética simples dos 3 canais
                uchar media = (pixel[0] + pixel[1] + pixel[2]) / 3;
                
                // Aplica o mesmo valor nos 3 canais (cria um pixel cinza)
                output.at<cv::Vec3b>(y, x) = cv::Vec3b(media, media, media);
            }
        }
    }
    return output;
}

/**
 * CONVERSÃO PARA TONS DE CINZA - MÉDIA PONDERADA (LUMINÂNCIA)
 * 
 * Funcionamento:
 * - Usa pesos diferentes para cada canal baseado na sensibilidade do olho humano
 * - Fórmula ITU-R BT.709: Cinza = 0.299*R + 0.587*G + 0.114*B
 * - Verde tem maior peso (0.587) porque o olho humano é mais sensível a ele
 * - Azul tem menor peso (0.114) porque contribui menos para a percepção de brilho
 * 
 * Vantagens: Resultado mais próximo da percepção visual humana
 * Desvantagens: Ligeiramente mais lento que a média simples
 */
cv::Mat Operation::toGrayscaleWeighted(const cv::Mat& input) {
    cv::Mat output = input.clone();
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            if (input.channels() == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                
                // Aplica os pesos padrão da ITU-R BT.709 para luminância
                // pixel[0] = Blue, pixel[1] = Green, pixel[2] = Red
                uchar weighted = static_cast<uchar>(0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
                
                output.at<cv::Vec3b>(y, x) = cv::Vec3b(weighted, weighted, weighted);
            }
        }
    }
    return output;
}

/**
 * CONVERSÃO PARA TONS DE CINZA REAL (1 CANAL) - SEM OPENCV
 * 
 * Funcionamento:
 * - Converte imagem colorida (3 canais) para escala de cinza real (1 canal)
 * - Usa média ponderada ITU-R BT.709 para melhor qualidade
 * - Implementação manual sem usar cv::cvtColor()
 */
cv::Mat Operation::toGrayscaleRealChannel(const cv::Mat& input) {
    if (input.channels() != 3) {
        std::cerr << "Erro: Imagem deve ter 3 canais para conversão!" << std::endl;
        return input.clone();
    }
    
    // Cria matriz de 1 canal
    cv::Mat output(input.rows, input.cols, CV_8UC1);
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
            
            // Aplica os pesos padrão da ITU-R BT.709 para luminância
            // pixel[0] = Blue, pixel[1] = Green, pixel[2] = Red
            uchar cinza = static_cast<uchar>(0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
            
            // Salva no formato de 1 canal
            output.at<uchar>(y, x) = cinza;
        }
    }
    return output;
}

/**
 * SOMA DE IMAGENS - OPERAÇÃO PIXEL A PIXEL
 * 
 * Funcionamento:
 * - Soma os valores correspondentes de cada canal de duas imagens
 * - Suporta combinações: colorida+colorida, colorida+cinza, cinza+cinza
 * - Para colorida+cinza: replica o valor cinza nos 3 canais BGR
 * - Trabalha com o menor tamanho comum entre as duas imagens
 */
cv::Mat Operation::addImages(const cv::Mat& img1, const cv::Mat& img2) {
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    cv::Mat result(rows, cols, img1.type());
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE OVERFLOW na soma de imagens
                    int sum = pixel1[c] + pixel2[c];
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(sum);
                }
            } else if (img1.channels() == 3 && img2.channels() == 1) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE OVERFLOW (colorida + cinza)
                    int sum = pixel1[c] + pixel2;
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(sum);
                }
            } else if (img1.channels() == 1 && img2.channels() == 3) {
                result = cv::Mat(rows, cols, img2.type());
                uchar pixel1 = img1.at<uchar>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE OVERFLOW (cinza + colorida)
                    int sum = pixel1 + pixel2[c];
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(sum);
                }
            } else if (img1.channels() == 1 && img2.channels() == 1) {
                result = cv::Mat(rows, cols, CV_8UC1);
                uchar pixel1 = img1.at<uchar>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                // TRATAMENTO DE OVERFLOW (cinza + cinza)
                int sum = pixel1 + pixel2;
                result.at<uchar>(y, x) = saturateValue(sum);
            }
        }
    }
    return result;
}

/**
 * SUBTRAÇÃO DE IMAGENS - OPERAÇÃO PIXEL A PIXEL
 * 
 * Funcionamento:
 * - Subtrai os valores correspondentes de cada canal
 * - Suporta combinações: colorida+colorida, colorida+cinza, cinza+cinza
 * - Para colorida+cinza: replica o valor cinza nos 3 canais BGR
 * - Valores negativos ficam 0 (clamp inferior)
 * 
 * Aplicações: Detecção de diferenças, remoção de fundo, detecção de movimento
 */
cv::Mat Operation::subtractImages(const cv::Mat& img1, const cv::Mat& img2) {
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    cv::Mat result(rows, cols, img1.type());
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE UNDERFLOW na subtração
                    int diff = pixel1[c] - pixel2[c];
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(diff);
                }
            } else if (img1.channels() == 3 && img2.channels() == 1) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE UNDERFLOW (colorida - cinza)
                    int diff = pixel1[c] - pixel2;
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(diff);
                }
            } else if (img1.channels() == 1 && img2.channels() == 3) {
                result = cv::Mat(rows, cols, img2.type());
                uchar pixel1 = img1.at<uchar>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE UNDERFLOW (cinza - colorida)
                    int diff = pixel1 - pixel2[c];
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(diff);
                }
            } else if (img1.channels() == 1 && img2.channels() == 1) {
                result = cv::Mat(rows, cols, CV_8UC1);
                uchar pixel1 = img1.at<uchar>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                // TRATAMENTO DE UNDERFLOW (cinza - cinza)
                int diff = pixel1 - pixel2;
                result.at<uchar>(y, x) = saturateValue(diff);
            }
        }
    }
    return result;
}

/**
 * MULTIPLICAÇÃO DE IMAGENS - OPERAÇÃO PIXEL A PIXEL
 * 
 * Funcionamento:
 * - Multiplica os valores correspondentes e divide por 255 para normalizar
 * - Suporta combinações: colorida+colorida, colorida+cinza, cinza+cinza
 * - Para colorida+cinza: replica o valor cinza nos 3 canais BGR
 * - A divisão por 255 mantém o resultado na faixa 0-255
 * 
 * Aplicações: Mascaramento, aplicação de texturas, efeitos de iluminação
 */
cv::Mat Operation::multiplyImages(const cv::Mat& img1, const cv::Mat& img2) {
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    cv::Mat result(rows, cols, img1.type());
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int prod = pixel1[c] * pixel2[c] / 255;
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(prod);
                }
            } else if (img1.channels() == 3 && img2.channels() == 1) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                for (int c = 0; c < 3; c++) {
                    int prod = pixel1[c] * pixel2 / 255;
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(prod);
                }
            } else if (img1.channels() == 1 && img2.channels() == 3) {
                result = cv::Mat(rows, cols, img2.type());
                uchar pixel1 = img1.at<uchar>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    int prod = pixel1 * pixel2[c] / 255;
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(prod);
                }
            } else if (img1.channels() == 1 && img2.channels() == 1) {
                result = cv::Mat(rows, cols, CV_8UC1);
                uchar pixel1 = img1.at<uchar>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                int prod = pixel1 * pixel2 / 255;
                result.at<uchar>(y, x) = saturateValue(prod);
            }
        }
    }
    return result;
}

/**
 * DIVISÃO DE IMAGENS - OPERAÇÃO PIXEL A PIXEL
 * 
 * Funcionamento:
 * - Divide um pixel pelo correspondente da segunda imagem
 * - Suporta combinações: colorida+colorida, colorida+cinza, cinza+cinza
 * - Para colorida+cinza: replica o valor cinza nos 3 canais BGR
 * - Multiplica por 255 para manter a faixa dinâmica
 * - Divisão por zero resulta em valor máximo (255)
 * 
 * Aplicações: Correção de iluminação não uniforme, normalização
 */
cv::Mat Operation::divideImages(const cv::Mat& img1, const cv::Mat& img2) {
    int rows = std::min(img1.rows, img2.rows);
    int cols = std::min(img1.cols, img2.cols);
    cv::Mat result(rows, cols, img1.type());
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (img1.channels() == 3 && img2.channels() == 3) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                cv::Vec3b pixel2 = img2.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    uchar denominator = pixel2[c];
                    uchar numerator = pixel1[c];
                    
                    // TRATAMENTO DE DIVISÃO POR ZERO pixel a pixel
                    if (denominator == 0) {
                        result.at<cv::Vec3b>(y, x)[c] = 255;  // Valor máximo para div/0
                    } else {
                        // Normalização com tratamento de overflow
                        int division = (numerator * 255) / denominator;
                        result.at<cv::Vec3b>(y, x)[c] = saturateValue(division);
                    }
                }
            } else if (img1.channels() == 3 && img2.channels() == 1) {
                cv::Vec3b pixel1 = img1.at<cv::Vec3b>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE DIVISÃO POR ZERO (colorida / cinza)
                    if (pixel2 == 0) {
                        result.at<cv::Vec3b>(y, x)[c] = 255;
                    } else {
                        int division = (pixel1[c] * 255) / pixel2;
                        result.at<cv::Vec3b>(y, x)[c] = saturateValue(division);
                    }
                }
            } else if (img1.channels() == 1 && img2.channels() == 1) {
                result = cv::Mat(rows, cols, CV_8UC1);
                uchar pixel1 = img1.at<uchar>(y, x);
                uchar pixel2 = img2.at<uchar>(y, x);
                
                // TRATAMENTO DE DIVISÃO POR ZERO (cinza / cinza)
                if (pixel2 == 0) {
                    result.at<uchar>(y, x) = 255;
                } else {
                    int division = (pixel1 * 255) / pixel2;
                    result.at<uchar>(y, x) = saturateValue(division);
                }
            }
        }
    }
    return result;
}

/**
 * SOMA COM ESCALAR - OPERAÇÃO ARITMÉTICA BÁSICA
 * 
 * Funcionamento:
 * - Adiciona um valor constante a todos os pixels da imagem
 * - Fórmula: Resultado(x,y) = Pixel(x,y) + Escalar
 * - Usar saturate_cast evita overflow automático
 * 
 * Aplicações: Ajuste de brilho (valores positivos clareiam a imagem)
 */
cv::Mat Operation::addScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                // TRATAMENTO DE OVERFLOW/UNDERFLOW na soma com escalar
                int sum = input.at<uchar>(y, x) + static_cast<int>(scalar);
                result.at<uchar>(y, x) = saturateValue(sum);
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE OVERFLOW/UNDERFLOW para cada canal
                    int sum = pixel[c] + static_cast<int>(scalar);
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(sum);
                }
            }
        }
    }
    return result;
}

/**
 * SUBTRAÇÃO COM ESCALAR - OPERAÇÃO ARITMÉTICA BÁSICA
 * 
 * Funcionamento:
 * - Subtrai um valor constante de todos os pixels
 * - Fórmula: Resultado(x,y) = Pixel(x,y) - Escalar
 * - Valores negativos ficam 0 (clamp inferior)
 * 
 * Aplicações: Redução de brilho (escurece a imagem)
 */
cv::Mat Operation::subtractScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                // TRATAMENTO DE UNDERFLOW na subtração
                int diff = input.at<uchar>(y, x) - static_cast<int>(scalar);
                result.at<uchar>(y, x) = saturateValue(diff);
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE UNDERFLOW para cada canal
                    int diff = pixel[c] - static_cast<int>(scalar);
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(diff);
                }
            }
        }
    }
    return result;
}

/**
 * MULTIPLICAÇÃO COM ESCALAR - AJUSTE DE CONTRASTE
 * 
 * Funcionamento:
 * - Multiplica todos os pixels por um fator constante
 * - Fórmula: Resultado(x,y) = Pixel(x,y) * Escalar
 * - Escalar > 1: Aumenta contraste e brilho
 * - Escalar < 1: Diminui contraste e brilho
 * 
 * Aplicações: Ajuste de contraste da imagem
 */
cv::Mat Operation::multiplyScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                // TRATAMENTO DE OVERFLOW na multiplicação
                double prod = input.at<uchar>(y, x) * scalar;
                result.at<uchar>(y, x) = saturateValue(static_cast<int>(prod));
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // TRATAMENTO DE OVERFLOW para cada canal
                    double prod = pixel[c] * scalar;
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(static_cast<int>(prod));
                }
            }
        }
    }
    return result;
}

/**
 * DIVISÃO COM ESCALAR - REDUÇÃO DE INTENSIDADE
 * 
 * Funcionamento:
 * - Divide todos os pixels por um fator constante
 * - Fórmula: Resultado(x,y) = Pixel(x,y) / Escalar
 * - Divisão por zero é tratada como valor máximo
 * 
 * Aplicações: Redução de brilho controlada, normalização
 */
cv::Mat Operation::divideScalar(const cv::Mat& input, double scalar) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    // TRATAMENTO DE DIVISÃO POR ZERO - Verificação preventiva
    if (std::abs(scalar) < 1e-10) {  // Considera valores muito próximos de zero
        std::cerr << "AVISO: Divisão por zero detectada! Valor escalar: " << scalar << std::endl;
        std::cerr << "Retornando imagem original sem modificações." << std::endl;
        return result;  // Retorna imagem original sem mudanças
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (channels == 1) {
                // Divisão segura com tratamento de precisão
                double quotient = input.at<uchar>(y, x) / scalar;
                result.at<uchar>(y, x) = saturateValue(static_cast<int>(quotient));
            } else if (channels == 3) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                for (int c = 0; c < 3; c++) {
                    // Divisão segura para cada canal
                    double quotient = pixel[c] / scalar;
                    result.at<cv::Vec3b>(y, x)[c] = saturateValue(static_cast<int>(quotient));
                }
            }
        }
    }
    return result;
}

/**
 * LIMIARIZAÇÃO (THRESHOLDING) - BINARIZAÇÃO DE IMAGEM
 * 
 * Funcionamento:
 * - Converte imagem em binária baseada em um valor de limiar
 * - Para imagens coloridas: primeiro converte para cinza, depois aplica limiar
 * - Fórmula: Se Pixel > Limiar então MaxVal, senão 0
 * 
 * Aplicações: Segmentação, detecção de objetos, pré-processamento para OCR
 */
cv::Mat Operation::threshold(const cv::Mat& input, double thresh, double maxval) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    if (channels == 1) {
        // Para imagens já em escala de cinza
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                uchar pixel = input.at<uchar>(y, x);
                // Aplica limiarização: pixel > limiar ? maxval : 0
                result.at<uchar>(y, x) = (pixel > thresh) ? static_cast<uchar>(maxval) : 0;
            }
        }
    } else if (channels == 3) {
        // Para imagens coloridas: converte para cinza primeiro
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                // Converte para cinza usando luminância
                uchar gray = static_cast<uchar>(0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
                // Aplica o resultado em todos os canais
                uchar value = (gray > thresh) ? static_cast<uchar>(maxval) : 0;
                result.at<cv::Vec3b>(y, x) = cv::Vec3b(value, value, value);
            }
        }
    }
    return result;
}

/**
 * ISOLAMENTO DE CANAL - EXTRAÇÃO DE COMPONENTE DE COR
 * 
 * Funcionamento:
 * - Mantém apenas um canal de cor, zerando os outros
 * - canal 0 = Azul, canal 1 = Verde, canal 2 = Vermelho
 * - Útil para análise de componentes individuais de cor
 * 
 * Aplicações: Análise de canais, debug, efeitos artísticos
 */
cv::Mat Operation::isolateChannel(const cv::Mat& input, int channel) {
    cv::Mat result = cv::Mat::zeros(input.size(), input.type());

    if (input.channels() == 3 && channel >= 0 && channel <= 2) {
        for (int y = 0; y < input.rows; y++) {
            for (int x = 0; x < input.cols; x++) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                cv::Vec3b newPixel(0, 0, 0);  // Inicia com zeros
                newPixel[channel] = pixel[channel];  // Mantém apenas o canal desejado
                result.at<cv::Vec3b>(y, x) = newPixel;
            }
        }
    } else if (input.channels() == 1 && channel == 0) {
        // Para imagens de 1 canal, apenas copia
        for (int y = 0; y < input.rows; y++) {
            for (int x = 0; x < input.cols; x++) {
                result.at<uchar>(y, x) = input.at<uchar>(y, x);
            }
        }
    }
    return result;
}

/**
 * CÁLCULO DE HISTOGRAMA - DISTRIBUIÇÃO DE INTENSIDADES
 * 
 * Funcionamento:
 * - Conta quantas vezes cada valor de intensidade (0-255) aparece na imagem
 * - Para imagens coloridas: calcula um histograma para cada canal (B, G, R)
 * - Resultado: vetor de matrizes onde cada posição [i] contém a contagem do valor i
 * 
 * Aplicações: Análise de distribuição tonal, equalização, ajuste de contraste
 */
std::vector<cv::Mat> Operation::computeHistogram(const cv::Mat& input) {
    std::vector<cv::Mat> histograms;
    int histSize = 256;  // 256 níveis de intensidade (0-255)

    if (input.channels() == 1) {
        // Para imagens em escala de cinza
        cv::Mat hist = cv::Mat::zeros(1, histSize, CV_32S);
        for (int y = 0; y < input.rows; y++) {
            for (int x = 0; x < input.cols; x++) {
                int val = input.at<uchar>(y, x);
                hist.at<int>(val)++;  // Incrementa contador para este valor
            }
        }
        histograms.push_back(hist);
    } else if (input.channels() == 3) {
        // Para imagens coloridas: um histograma por canal
        for (int c = 0; c < 3; c++) {
            cv::Mat hist = cv::Mat::zeros(1, histSize, CV_32S);
            for (int y = 0; y < input.rows; y++) {
                for (int x = 0; x < input.cols; x++) {
                    int val = input.at<cv::Vec3b>(y, x)[c];
                    hist.at<int>(val)++;
                }
            }
            histograms.push_back(hist);
        }
    }
    return histograms;
}

/**
 * VISUALIZAÇÃO DE HISTOGRAMA - REPRESENTAÇÃO GRÁFICA CORRIGIDA
 */
cv::Mat Operation::visualizeHistogram(const std::vector<cv::Mat>& histograms) {
    int hist_w = 512, hist_h = 400;
    int bin_w = hist_w / 256;  // Usando divisão inteira em vez de cvRound
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    // Encontra o valor máximo para normalização
    int maxVal = 0;
    for (const auto& hist : histograms) {
        for (int i = 0; i < 256; i++) {
            maxVal = std::max(maxVal, hist.at<int>(i));
        }
    }
    
    // TRATAMENTO DE DIVISÃO POR ZERO na normalização
    if (maxVal == 0) {
        std::cerr << "AVISO: Histograma vazio (maxVal = 0)! Definindo maxVal = 1." << std::endl;
        maxVal = 1;
    }

    // Normaliza e desenha cada histograma
    for (size_t c = 0; c < histograms.size(); c++) {
        cv::Scalar color;
        if (histograms.size() == 1) {
            color = cv::Scalar(255, 255, 255);
        } else {
            if (c == 0) color = cv::Scalar(255, 0, 0);      // Azul
            else if (c == 1) color = cv::Scalar(0, 255, 0); // Verde  
            else color = cv::Scalar(0, 0, 255);             // Vermelho
        }

        // Desenha linhas do histograma com tratamento de overflow
        for (int i = 1; i < 256; i++) {
            // TRATAMENTO DE OVERFLOW na normalização de altura
            int y1 = hist_h - roundToInt((double)histograms[c].at<int>(i-1) * hist_h / maxVal);
            int y2 = hist_h - roundToInt((double)histograms[c].at<int>(i) * hist_h / maxVal);
            
            // Garante que y1 e y2 estejam dentro dos limites
            y1 = std::max(0, std::min(hist_h, y1));
            y2 = std::max(0, std::min(hist_h, y2));
            
            cv::line(histImage,
                cv::Point(bin_w * (i-1), y1),
                cv::Point(bin_w * i, y2),
                color, 1, cv::LINE_AA);
        }
    }
    return histImage;
}

/**
 * INVERSÃO DE IMAGEM - NEGATIVO FOTOGRÁFICO
 * 
 * Funcionamento:
 * - Inverte os valores de intensidade de cada pixel
 * - Fórmula: Resultado(x,y) = 255 - Pixel(x,y)
 * - Pixels claros ficam escuros e vice-versa
 * 
 * Aplicações: Efeito negativo, realce de detalhes escuros, efeitos artísticos
 */
cv::Mat Operation::invert(const cv::Mat& input) {
    cv::Mat result = input.clone();
    int rows = input.rows;
    int cols = input.cols;
    int channels = input.channels();

    if (channels == 1) {
        // Para imagens em escala de cinza
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                result.at<uchar>(y, x) = 255 - input.at<uchar>(y, x);
            }
        }
    } else if (channels == 3) {
        // Para imagens coloridas (inverte cada canal)
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
                result.at<cv::Vec3b>(y, x) = cv::Vec3b(255 - pixel[0], 255 - pixel[1], 255 - pixel[2]);
            }
        }
    }
    return result;
}

/**
 * MÉTODO DE TESTE ESPECÍFICO PARA HISTOGRAMAS
 */
void Operation::testHistogramImages() {
    std::cout << "\n=== TESTE ESPECÍFICO DE HISTOGRAMA ===" << std::endl;
    
    // Teste Colorido1
    cv::Mat colorido1 = cv::imread("../data/model/Colorida1.jpeg");
    if (!colorido1.empty()) {
        std::vector<cv::Mat> hist1 = computeHistogram(colorido1);
        cv::Mat histVis1 = visualizeHistogram(hist1);
        cv::imwrite("../result/teste_histograma_colorido1.jpg", histVis1);
        std::cout << "Histograma Colorido1 salvo em: result/teste_histograma_colorido1.jpg" << std::endl;
    }
    
    // Teste Colorido2  
    cv::Mat colorido2 = cv::imread("../data/model/Colorida2.jpeg");
    if (!colorido2.empty()) {
        std::vector<cv::Mat> hist2 = computeHistogram(colorido2);
        cv::Mat histVis2 = visualizeHistogram(hist2);
        cv::imwrite("../result/teste_histograma_colorido2.jpg", histVis2);
        std::cout << "Histograma Colorido2 salvo em: result/teste_histograma_colorido2.jpg" << std::endl;
    }
    
    // Teste imagem em tons de cinza
    cv::Mat cinzaEColorido = cv::imread("../data/model/CinzaEColorido.jpeg");
    if (!cinzaEColorido.empty()) {
        std::vector<cv::Mat> hist3 = computeHistogram(cinzaEColorido);
        cv::Mat histVis3 = visualizeHistogram(hist3);
        cv::imwrite("../result/teste_histograma_cinzaecolorido.jpg", histVis3);
        std::cout << "Histograma CinzaEColorido salvo em: result/teste_histograma_cinzaecolorido.jpg" << std::endl;
    }
}