#include <iostream>
#include <opencv2/opencv.hpp>
#include "Operation.hpp"
#include <filesystem>

int main() {
    std::cout << "=== DEMONSTRAÇÃO DAS FUNCIONALIDADES PDI ===" << std::endl;
    
    // Criar pasta result se não existir
    std::filesystem::create_directories("../result");

    // 1. Conversão para tons de cinza - usar imagem colorida específica
    std::cout << "\n1. Conversão para tons de cinza (Colorido 1)..." << std::endl;
    cv::Mat colorido1 = cv::imread("../data/model/Colorida1.jpeg");
    if (colorido1.empty()) {
        std::cerr << "Erro ao abrir ../data/model/Colorida1.jpeg!" << std::endl;
        return 1;
    }
    
    std::cout << "Imagem Colorido1: " << colorido1.cols << "x" << colorido1.rows << " pixels, " << colorido1.channels() << " canais" << std::endl;
    
    cv::Mat grayAverage = Operation::toGrayscaleAverage(colorido1);
    cv::Mat grayWeighted = Operation::toGrayscaleWeighted(colorido1);
    
    cv::imwrite("../result/gray_average_colorido1.jpg", grayAverage);
    cv::imwrite("../result/gray_weighted_colorido1.jpg", grayWeighted);
    std::cout << "   - Média aritmética salva como result/gray_average_colorido1.jpg" << std::endl;
    std::cout << "   - Média ponderada salva como result/gray_weighted_colorido1.jpg" << std::endl;

    // 2. Operações aritméticas com escalar - usar primeira imagem disponível
    std::cout << "\n2. Operações aritméticas com escalar..." << std::endl;
    cv::Mat brightened = Operation::addScalar(colorido1, 50);
    cv::Mat darkened = Operation::subtractScalar(colorido1, 30);
    cv::Mat contrast = Operation::multiplyScalar(colorido1, 1.5);
    cv::Mat reduced = Operation::divideScalar(colorido1, 2.0);
    
    cv::imwrite("../result/brightened.jpg", brightened);
    cv::imwrite("../result/darkened.jpg", darkened);
    cv::imwrite("../result/contrast.jpg", contrast);
    cv::imwrite("../result/reduced.jpg", reduced);
    std::cout << "   - Imagem clarificada (+50) salva como result/brightened.jpg" << std::endl;
    std::cout << "   - Imagem escurecida (-30) salva como result/darkened.jpg" << std::endl;
    std::cout << "   - Contraste aumentado (*1.5) salvo como result/contrast.jpg" << std::endl;
    std::cout << "   - Intensidade reduzida (/2) salva como result/reduced.jpg" << std::endl;

    // 3. Operações aritméticas entre imagens
    std::cout << "\n3. Operações aritméticas entre imagens..." << std::endl;
    cv::Mat added = Operation::addImages(colorido1, grayWeighted);
    cv::Mat subtracted = Operation::subtractImages(colorido1, grayWeighted);
    cv::Mat multiplied = Operation::multiplyImages(colorido1, grayWeighted);
    cv::Mat divided = Operation::divideImages(colorido1, grayWeighted);
    
    cv::imwrite("../result/added.jpg", added);
    cv::imwrite("../result/subtracted.jpg", subtracted);
    cv::imwrite("../result/multiplied.jpg", multiplied);
    cv::imwrite("../result/divided.jpg", divided);
    std::cout << "   - Soma de imagens salva como result/added.jpg" << std::endl;
    std::cout << "   - Subtração de imagens salva como result/subtracted.jpg" << std::endl;
    std::cout << "   - Multiplicação de imagens salva como result/multiplied.jpg" << std::endl;
    std::cout << "   - Divisão de imagens salva como result/divided.jpg" << std::endl;

    // 4. Limiarização - usar imagem CinzaEColorido
    std::cout << "\n4. Limiarização (CinzaEColorido)..." << std::endl;
    cv::Mat cinzaEColorido = cv::imread("../data/model/CinzaEColorido.jpeg");
    if (cinzaEColorido.empty()) {
        std::cerr << "Erro ao abrir ../data/model/CinzaEColorido.jpeg!" << std::endl;
        return 1;
    }
    
    std::cout << "Imagem CinzaEColorido: " << cinzaEColorido.cols << "x" << cinzaEColorido.rows << " pixels, " << cinzaEColorido.channels() << " canais" << std::endl;
    
    cv::Mat thresholded128 = Operation::threshold(cinzaEColorido, 128, 255);
    cv::Mat thresholded80 = Operation::threshold(cinzaEColorido, 80, 255);
    cv::Mat thresholded180 = Operation::threshold(cinzaEColorido, 180, 255);
    
    // Teste também com versão em cinza
    cv::Mat cinzaEColoridoGray;
    cv::cvtColor(cinzaEColorido, cinzaEColoridoGray, cv::COLOR_BGR2GRAY);
    cv::Mat thresholdedGray128 = Operation::threshold(cinzaEColoridoGray, 128, 255);
    
    cv::imwrite("../result/thresholded_128_cinzaecolorido.jpg", thresholded128);
    cv::imwrite("../result/thresholded_80_cinzaecolorido.jpg", thresholded80);
    cv::imwrite("../result/thresholded_180_cinzaecolorido.jpg", thresholded180);
    cv::imwrite("../result/thresholded_gray_128_cinzaecolorido.jpg", thresholdedGray128);
    std::cout << "   - Limiarização colorida (limiar=128) salva como result/thresholded_128_cinzaecolorido.jpg" << std::endl;
    std::cout << "   - Limiarização colorida (limiar=80) salva como result/thresholded_80_cinzaecolorido.jpg" << std::endl;
    std::cout << "   - Limiarização colorida (limiar=180) salva como result/thresholded_180_cinzaecolorido.jpg" << std::endl;
    std::cout << "   - Limiarização cinza (limiar=128) salva como result/thresholded_gray_128_cinzaecolorido.jpg" << std::endl;

    // 5. Isolamento de canais - usar primeira imagem colorida
    std::cout << "\n5. Isolamento de canais..." << std::endl;
    cv::Mat channelB = Operation::isolateChannel(colorido1, 0); // Canal Azul
    cv::Mat channelG = Operation::isolateChannel(colorido1, 1); // Canal Verde
    cv::Mat channelR = Operation::isolateChannel(colorido1, 2); // Canal Vermelho
    
    cv::imwrite("../result/channel_blue.jpg", channelB);
    cv::imwrite("../result/channel_green.jpg", channelG);
    cv::imwrite("../result/channel_red.jpg", channelR);
    std::cout << "   - Canal azul isolado salvo como result/channel_blue.jpg" << std::endl;
    std::cout << "   - Canal verde isolado salvo como result/channel_green.jpg" << std::endl;
    std::cout << "   - Canal vermelho isolado salvo como result/channel_red.jpg" << std::endl;

    // 6. Histograma - usar imagem Colorido 2
    std::cout << "\n6. Computação e visualização de histograma (Colorido 2)..." << std::endl;
    cv::Mat colorido2 = cv::imread("../data/model/Colorida2.jpeg");
    if (colorido2.empty()) {
        std::cerr << "Erro ao abrir ../data/model/Colorida2.jpeg!" << std::endl;
        return 1;
    }
    
    std::cout << "Imagem Colorido2: " << colorido2.cols << "x" << colorido2.rows << " pixels, " << colorido2.channels() << " canais" << std::endl;
    
    // Histograma da imagem colorida
    std::vector<cv::Mat> histColor = Operation::computeHistogram(colorido2);
    cv::Mat histImageColor = Operation::visualizeHistogram(histColor);
    cv::imwrite("../result/histogram_color_colorido2.jpg", histImageColor);
    std::cout << "   - Histograma colorido salvo como result/histogram_color_colorido2.jpg" << std::endl;

    // Histograma para escala de cinza
    cv::Mat colorido2Gray;
    cv::cvtColor(colorido2, colorido2Gray, cv::COLOR_BGR2GRAY);
    std::vector<cv::Mat> histGray = Operation::computeHistogram(colorido2Gray);
    cv::Mat histImageGray = Operation::visualizeHistogram(histGray);
    cv::imwrite("../result/histogram_gray_colorido2.jpg", histImageGray);
    std::cout << "   - Histograma em escala de cinza salvo como result/histogram_gray_colorido2.jpg" << std::endl;

    // Equalização de histograma
    cv::Mat equalizedColor = Operation::equalizeHistogram(colorido2);
    cv::Mat equalizedGray = Operation::equalizeHistogram(colorido2Gray);
    cv::imwrite("../result/equalized_color_colorido2.jpg", equalizedColor);
    cv::imwrite("../result/equalized_gray_colorido2.jpg", equalizedGray);
    std::cout << "   - Equalização colorida salva como result/equalized_color_colorido2.jpg" << std::endl;
    std::cout << "   - Equalização em cinza salva como result/equalized_gray_colorido2.jpg" << std::endl;

    // 7. Inverso da imagem - usar imagem Inverso
    std::cout << "\n7. Inverso da imagem (Inverso)..." << std::endl;
    cv::Mat inverso = cv::imread("../data/model/Inverso.jpeg");
    if (inverso.empty()) {
        std::cerr << "Erro ao abrir ../data/model/Inverso.jpeg!" << std::endl;
        return 1;
    }
    
    std::cout << "Imagem Inverso: " << inverso.cols << "x" << inverso.rows << " pixels, " << inverso.channels() << " canais" << std::endl;
    
    cv::Mat inverted = Operation::invert(inverso);
    cv::Mat inversoGray;
    cv::cvtColor(inverso, inversoGray, cv::COLOR_BGR2GRAY);
    cv::Mat invertedGray = Operation::invert(inversoGray);
    
    cv::imwrite("../result/inverted_inverso.jpg", inverted);
    cv::imwrite("../result/inverted_gray_inverso.jpg", invertedGray);
    std::cout << "   - Imagem invertida (colorida) salva como result/inverted_inverso.jpg" << std::endl;
    std::cout << "   - Imagem invertida (cinza) salva como result/inverted_gray_inverso.jpg" << std::endl;

    // Exibir algumas imagens principais para demonstração
    std::cout << "\n=== EXIBINDO RESULTADOS ===" << std::endl;
    cv::imshow("Colorido1 - Original", colorido1);
    cv::imshow("Colorido1 - Média Aritmética", grayAverage);
    cv::imshow("Colorido1 - Média Ponderada", grayWeighted);
    cv::imshow("Colorido1 - Canal Vermelho", channelR);
    
    cv::imshow("CinzaEColorido - Original", cinzaEColorido);
    cv::imshow("CinzaEColorido - Limiarizada", thresholded128);
    
    cv::imshow("Colorido2 - Original", colorido2);
    cv::imshow("Colorido2 - Histograma", histImageColor);
    cv::imshow("Colorido2 - Equalizada", equalizedColor);
    
    cv::imshow("Inverso - Original", inverso);
    cv::imshow("Inverso - Invertida", inverted);

    std::cout << "\nPressione qualquer tecla para sair..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    std::cout << "\n=== RESUMO ===" << std::endl;
    std::cout << "✓ Conversão para tons de cinza (Colorido1) - média aritmética e ponderada" << std::endl;
    std::cout << "✓ Operações aritméticas (imagem-imagem e imagem-escalar)" << std::endl;
    std::cout << "✓ Limiarização (CinzaEColorido) - múltiplos limiares para colorida e cinza" << std::endl;
    std::cout << "✓ Isolamento de canais RGB" << std::endl;
    std::cout << "✓ Computação e visualização de histogramas (Colorido2)" << std::endl;
    std::cout << "✓ Equalização de histograma (Colorido2)" << std::endl;
    std::cout << "✓ Inverso da imagem (Inverso)" << std::endl;
    std::cout << "\nTodas as imagens foram salvas na pasta result/" << std::endl;
    std::cout << "Processamento concluído com sucesso!" << std::endl;

    return 0;
}