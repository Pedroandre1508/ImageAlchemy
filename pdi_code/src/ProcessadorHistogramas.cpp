#include "ProcessadorHistogramas.hpp"
#include <algorithm>
#include <cmath>

// Função auxiliar para arredondamento manual
inline int roundToInt(double value) {
    return static_cast<int>(value + 0.5);
}

std::vector<cv::Mat> ProcessadorHistogramas::calcularHistograma(const cv::Mat& imagem) {
    std::vector<cv::Mat> histogramas;
    int tamanhoHist = 256;

    if (imagem.channels() == 1) {
        cv::Mat hist = cv::Mat::zeros(1, tamanhoHist, CV_32S);
        for (int y = 0; y < imagem.rows; y++) {
            for (int x = 0; x < imagem.cols; x++) {
                int valor = imagem.at<uchar>(y, x);
                hist.at<int>(valor)++;
            }
        }
        histogramas.push_back(hist);
    } else if (imagem.channels() == 3) {
        for (int c = 0; c < 3; c++) {
            cv::Mat hist = cv::Mat::zeros(1, tamanhoHist, CV_32S);
            for (int y = 0; y < imagem.rows; y++) {
                for (int x = 0; x < imagem.cols; x++) {
                    int valor = imagem.at<cv::Vec3b>(y, x)[c];
                    hist.at<int>(valor)++;
                }
            }
            histogramas.push_back(hist);
        }
    }
    return histogramas;
}

cv::Mat ProcessadorHistogramas::visualizarHistograma(const std::vector<cv::Mat>& histogramas) {
    int largura = 512, altura = 400;
    int larguraBin = largura / 256;
    cv::Mat imagemHist(altura, largura, CV_8UC3, cv::Scalar(0, 0, 0));

    // Encontrar valor máximo para normalização
    int valorMaximo = 0;
    for (const auto& hist : histogramas) {
        for (int i = 0; i < 256; i++) {
            valorMaximo = std::max(valorMaximo, hist.at<int>(i));
        }
    }
    if (valorMaximo == 0) valorMaximo = 1;

    for (size_t c = 0; c < histogramas.size(); c++) {
        cv::Scalar cor;
        if (histogramas.size() == 1) cor = cv::Scalar(255, 255, 255);
        else if (c == 0) cor = cv::Scalar(255, 0, 0); // Azul
        else if (c == 1) cor = cv::Scalar(0, 255, 0); // Verde
        else cor = cv::Scalar(0, 0, 255); // Vermelho

        for (int i = 1; i < 256; i++) {
            int h1 = roundToInt((double)histogramas[c].at<int>(i - 1) * altura / valorMaximo);
            int h2 = roundToInt((double)histogramas[c].at<int>(i) * altura / valorMaximo);
            cv::line(imagemHist,
                cv::Point(larguraBin * (i - 1), altura - h1),
                cv::Point(larguraBin * i, altura - h2),
                cor, 2, 8, 0);
        }
    }
    return imagemHist;
}

cv::Mat ProcessadorHistogramas::equalizarHistograma(const cv::Mat& imagem) {
    cv::Mat resultado = imagem.clone();
    
    if (imagem.channels() == 1) {
        // Calcular histograma
        int hist[256] = {0};
        for (int y = 0; y < imagem.rows; y++) {
            for (int x = 0; x < imagem.cols; x++) {
                hist[imagem.at<uchar>(y, x)]++;
            }
        }

        // Calcular CDF (Função de Distribuição Cumulativa)
        int cdf[256] = {0};
        cdf[0] = hist[0];
        for (int i = 1; i < 256; i++) {
            cdf[i] = cdf[i - 1] + hist[i];
        }

        // Encontrar CDF mínimo
        int cdfMin = 0;
        for (int i = 0; i < 256; i++) {
            if (cdf[i] != 0) {
                cdfMin = cdf[i];
                break;
            }
        }

        // Criar tabela de lookup
        int totalPixels = imagem.rows * imagem.cols;
        uchar lut[256];
        for (int i = 0; i < 256; i++) {
            lut[i] = static_cast<uchar>(roundToInt((cdf[i] - cdfMin) * 255.0 / (totalPixels - cdfMin)));
        }

        // Aplicar equalização
        for (int y = 0; y < imagem.rows; y++) {
            for (int x = 0; x < imagem.cols; x++) {
                resultado.at<uchar>(y, x) = lut[imagem.at<uchar>(y, x)];
            }
        }
    } else if (imagem.channels() == 3) {
        // Equalizar cada canal separadamente
        for (int c = 0; c < 3; c++) {
            int hist[256] = {0};
            for (int y = 0; y < imagem.rows; y++) {
                for (int x = 0; x < imagem.cols; x++) {
                    hist[imagem.at<cv::Vec3b>(y, x)[c]]++;
                }
            }

            int cdf[256] = {0};
            cdf[0] = hist[0];
            for (int i = 1; i < 256; i++) {
                cdf[i] = cdf[i - 1] + hist[i];
            }

            int cdfMin = 0;
            for (int i = 0; i < 256; i++) {
                if (cdf[i] != 0) {
                    cdfMin = cdf[i];
                    break;
                }
            }

            int totalPixels = imagem.rows * imagem.cols;
            uchar lut[256];
            for (int i = 0; i < 256; i++) {
                lut[i] = static_cast<uchar>(roundToInt((cdf[i] - cdfMin) * 255.0 / (totalPixels - cdfMin)));
            }

            for (int y = 0; y < imagem.rows; y++) {
                for (int x = 0; x < imagem.cols; x++) {
                    resultado.at<cv::Vec3b>(y, x)[c] = lut[imagem.at<cv::Vec3b>(y, x)[c]];
                }
            }
        }
    }
    return resultado;
}
