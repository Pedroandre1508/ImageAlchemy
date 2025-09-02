#include <iostream>
#include <opencv2/opencv.hpp>
#include "Operation.hpp"
#include <filesystem>

/**
 * FUNÇÃO AUXILIAR PARA EXIBIÇÃO DE IMAGENS
 * 
 * Funcionamento:
 * - Redimensiona automaticamente imagens grandes para caber na tela
 * - Mantém proporção original da imagem
 * - Cria janela com nome personalizado
 * - Controla tempo de exibição
 */
void mostrarImagem(const std::string& nomeJanela, const cv::Mat& imagem, int tempoEspera = 0) {
    // Configurações de tamanho máximo para as janelas
    cv::Mat imagemRedimensionada;
    int larguraMax = 600;
    int alturaMax = 450;
    
    // Verifica se a imagem é maior que o tamanho máximo permitido
    if (imagem.cols > larguraMax || imagem.rows > alturaMax) {
        // Calcula o fator de escala mantendo a proporção
        double escala = std::min((double)larguraMax / imagem.cols, (double)alturaMax / imagem.rows);
        // Redimensiona usando interpolação bilinear (boa qualidade)
        cv::resize(imagem, imagemRedimensionada, cv::Size(0, 0), escala, escala, cv::INTER_AREA);
    } else {
        // Se a imagem já é pequena, usa o tamanho original
        imagemRedimensionada = imagem.clone();
    }
    
    // Cria janela com tamanho automático baseado na imagem
    cv::namedWindow(nomeJanela, cv::WINDOW_AUTOSIZE);
    cv::imshow(nomeJanela, imagemRedimensionada);
}

/**
 * PROGRAMA PRINCIPAL - DEMONSTRAÇÃO COMPLETA DE PDI
 * 
 * Estrutura:
 * 1. Conversões para tons de cinza
 * 2. Operações aritméticas com escalares
 * 3. Operações aritméticas entre imagens
 * 4. Limiarização (binarização)
 * 5. Isolamento de canais de cor
 * 6. Análise e equalização de histograma
 * 7. Inversão de imagem
 */
int main() {
    std::cout << "=== DEMONSTRAÇÃO DAS FUNCIONALIDADES PDI ===" << std::endl;
    std::cout << "Pressione qualquer tecla para continuar entre as visualizações..." << std::endl;
    
    // Cria pasta de resultados se não existir
    std::filesystem::create_directories("../result");

    // =============================================================================
    // 1. CONVERSÃO PARA TONS DE CINZA - DEMONSTRAÇÃO DE DOIS MÉTODOS
    // =============================================================================
    std::cout << "\n1. Conversão para tons de cinza (Colorido 1)..." << std::endl;
    cv::Mat imagemColorida1 = cv::imread("../data/model/Colorida1.jpeg");
    if (imagemColorida1.empty()) {
        std::cerr << "Erro ao abrir ../data/model/Colorida1.jpeg!" << std::endl;
        return 1;
    }
    
    // Carrega a segunda imagem colorida para operações entre imagens
    cv::Mat imagemColorida2 = cv::imread("../data/model/Colorida2.jpeg");
    if (imagemColorida2.empty()) {
        std::cerr << "Erro ao abrir ../data/model/Colorida2.jpeg!" << std::endl;
        return 1;
    }
    
    std::cout << "Imagem Colorido1: " << imagemColorida1.cols << "x" << imagemColorida1.rows << " pixels, " << imagemColorida1.channels() << " canais" << std::endl;
    std::cout << "Imagem Colorido2: " << imagemColorida2.cols << "x" << imagemColorida2.rows << " pixels, " << imagemColorida2.channels() << " canais" << std::endl;
    
    // Exibe a imagem original para comparação
    mostrarImagem("Original - Colorida1", imagemColorida1, 3000);
    
    // Aplica as duas técnicas de conversão para cinza
    cv::Mat cinzaMediaAritmetica = Operation::toGrayscaleAverage(imagemColorida1);
    cv::Mat cinzaMediaPonderada = Operation::toGrayscaleWeighted(imagemColorida1);
    
    // Exibe os resultados para comparação visual
    mostrarImagem("Conversão - Média Aritmética", cinzaMediaAritmetica, 3000);
    mostrarImagem("Conversão - Média Ponderada", cinzaMediaPonderada, 3000);
    
    // Salva os resultados
    cv::imwrite("../result/cinza_media_aritmetica_colorido1.jpg", cinzaMediaAritmetica);
    cv::imwrite("../result/cinza_media_ponderada_colorido1.jpg", cinzaMediaPonderada);
    std::cout << "   - Média aritmética salva como result/cinza_media_aritmetica_colorido1.jpg" << std::endl;
    std::cout << "   - Média ponderada salva como result/cinza_media_ponderada_colorido1.jpg" << std::endl;

    // =============================================================================
    // 2. OPERAÇÕES ARITMÉTICAS COM ESCALAR - AJUSTES GLOBAIS DE IMAGEM
    // =============================================================================
    std::cout << "\n2. Operações aritméticas com escalar..." << std::endl;
    
    // Aplica diferentes operações para demonstrar efeitos
    cv::Mat imagemClareada = Operation::addScalar(imagemColorida1, 50);      // +50: Clareia
    cv::Mat imagemEscurecida = Operation::subtractScalar(imagemColorida1, 30); // -30: Escurece
    cv::Mat imagemContraste = Operation::multiplyScalar(imagemColorida1, 1.5); // *1.5: Aumenta contraste
    cv::Mat imagemReduzida = Operation::divideScalar(imagemColorida1, 2.0);    // /2: Reduz intensidade
    
    // Exibe sequência comparativa
    mostrarImagem("Original", imagemColorida1, 2000);
    mostrarImagem("Clareada (+50)", imagemClareada, 2000);
    mostrarImagem("Escurecida (-30)", imagemEscurecida, 2000);
    mostrarImagem("Contraste (*1.5)", imagemContraste, 2000);
    mostrarImagem("Reduzida (/2)", imagemReduzida, 2000);
    
    // Salva todos os resultados
    cv::imwrite("../result/imagem_clareada.jpg", imagemClareada);
    cv::imwrite("../result/imagem_escurecida.jpg", imagemEscurecida);
    cv::imwrite("../result/imagem_contraste.jpg", imagemContraste);
    cv::imwrite("../result/imagem_reduzida.jpg", imagemReduzida);
    std::cout << "   - Imagem clarificada (+50) salva como result/imagem_clareada.jpg" << std::endl;
    std::cout << "   - Imagem escurecida (-30) salva como result/imagem_escurecida.jpg" << std::endl;
    std::cout << "   - Contraste aumentado (*1.5) salvo como result/imagem_contraste.jpg" << std::endl;
    std::cout << "   - Intensidade reduzida (/2) salva como result/imagem_reduzida.jpg" << std::endl;

    // =============================================================================
    // 3. OPERAÇÕES ARITMÉTICAS ENTRE IMAGENS - DEMONSTRAÇÃO COMPLETA
    // =============================================================================
    std::cout << "\n3. Operações aritméticas entre imagens..." << std::endl;
    
    // Converte Colorida1 para tons de cinza real (1 canal) para demonstrar operações mistas
    cv::Mat cinzaColorida1 = Operation::toGrayscaleWeighted(imagemColorida1);
    cv::Mat cinzaReal;
    cv::cvtColor(cinzaColorida1, cinzaReal, cv::COLOR_BGR2GRAY);
    
    std::cout << "\n   3.1 Operações Colorida com Colorida (Colorida1 + Colorida2)..." << std::endl;
    // Operações entre duas imagens coloridas diferentes
    cv::Mat somaCores = Operation::addImages(imagemColorida1, imagemColorida2);
    cv::Mat subtracaoCores = Operation::subtractImages(imagemColorida1, imagemColorida2);
    cv::Mat multiplicacaoCores = Operation::multiplyImages(imagemColorida1, imagemColorida2);
    cv::Mat divisaoCores = Operation::divideImages(imagemColorida1, imagemColorida2);
    
    // Exibe operações colorida com colorida
    mostrarImagem("Colorida1 + Colorida2", somaCores, 2000);
    mostrarImagem("Colorida1 - Colorida2", subtracaoCores, 2000);
    mostrarImagem("Colorida1 * Colorida2", multiplicacaoCores, 2000);
    mostrarImagem("Colorida1 / Colorida2", divisaoCores, 2000);
    
    // Salva operações colorida com colorida
    cv::imwrite("../result/soma_colorida1_colorida2.jpg", somaCores);
    cv::imwrite("../result/subtracao_colorida1_colorida2.jpg", subtracaoCores);
    cv::imwrite("../result/multiplicacao_colorida1_colorida2.jpg", multiplicacaoCores);
    cv::imwrite("../result/divisao_colorida1_colorida2.jpg", divisaoCores);
    
    std::cout << "   - Soma Colorida1+Colorida2 salva como result/soma_colorida1_colorida2.jpg" << std::endl;
    std::cout << "   - Subtração Colorida1-Colorida2 salva como result/subtracao_colorida1_colorida2.jpg" << std::endl;
    std::cout << "   - Multiplicação Colorida1*Colorida2 salva como result/multiplicacao_colorida1_colorida2.jpg" << std::endl;
    std::cout << "   - Divisão Colorida1/Colorida2 salva como result/divisao_colorida1_colorida2.jpg" << std::endl;
    
    std::cout << "\n   3.2 Operações Colorida com Tons de Cinza..." << std::endl;
    // Demonstra operações entre imagem colorida e imagem em tons de cinza
    cv::Mat somaColoridaCinza = Operation::addImages(imagemColorida1, cinzaReal);
    cv::Mat subtracaoColoridaCinza = Operation::subtractImages(imagemColorida1, cinzaReal);
    cv::Mat multiplicacaoColoridaCinza = Operation::multiplyImages(imagemColorida1, cinzaReal);
    cv::Mat divisaoColoridaCinza = Operation::divideImages(imagemColorida1, cinzaReal);
    
    // Exibe operações colorida com cinza
    mostrarImagem("Cinza (1 canal)", cinzaReal, 2000);
    mostrarImagem("Colorida + Cinza", somaColoridaCinza, 2000);
    mostrarImagem("Colorida - Cinza", subtracaoColoridaCinza, 2000);
    mostrarImagem("Colorida * Cinza", multiplicacaoColoridaCinza, 2000);
    mostrarImagem("Colorida / Cinza", divisaoColoridaCinza, 2000);
    
    // Salva operações colorida com cinza
    cv::imwrite("../result/cinza_1canal.jpg", cinzaReal);
    cv::imwrite("../result/soma_colorida_cinza.jpg", somaColoridaCinza);
    cv::imwrite("../result/subtracao_colorida_cinza.jpg", subtracaoColoridaCinza);
    cv::imwrite("../result/multiplicacao_colorida_cinza.jpg", multiplicacaoColoridaCinza);
    cv::imwrite("../result/divisao_colorida_cinza.jpg", divisaoColoridaCinza);
    
    std::cout << "   - Imagem cinza (1 canal) salva como result/cinza_1canal.jpg" << std::endl;
    std::cout << "   - Soma Colorida+Cinza salva como result/soma_colorida_cinza.jpg" << std::endl;
    std::cout << "   - Subtração Colorida-Cinza salva como result/subtracao_colorida_cinza.jpg" << std::endl;
    std::cout << "   - Multiplicação Colorida*Cinza salva como result/multiplicacao_colorida_cinza.jpg" << std::endl;
    std::cout << "   - Divisão Colorida/Cinza salva como result/divisao_colorida_cinza.jpg" << std::endl;

    // =============================================================================
    // 4. LIMIARIZAÇÃO - BINARIZAÇÃO PARA SEGMENTAÇÃO
    // =============================================================================
    std::cout << "\n4. Limiarização (CinzaEColorido)..." << std::endl;
    cv::Mat imagemCinzaEColorido = cv::imread("../data/model/CinzaEColorido.jpeg");
    if (imagemCinzaEColorido.empty()) {
        std::cerr << "Erro ao abrir ../data/model/CinzaEColorido.jpeg!" << std::endl;
        return 1;
    }
    
    std::cout << "Imagem CinzaEColorido: " << imagemCinzaEColorido.cols << "x" << imagemCinzaEColorido.rows << " pixels, " << imagemCinzaEColorido.channels() << " canais" << std::endl;
    
    // Aplica diferentes valores de limiar para demonstrar o efeito
    cv::Mat limiarizada128 = Operation::threshold(imagemCinzaEColorido, 128, 255); // Limiar médio
    cv::Mat limiarizada80 = Operation::threshold(imagemCinzaEColorido, 80, 255);   // Limiar baixo (mais branco)
    cv::Mat limiarizada180 = Operation::threshold(imagemCinzaEColorido, 180, 255); // Limiar alto (mais preto)
    
    // Mostra a progressão dos diferentes limiares
    mostrarImagem("Original - CinzaEColorido", imagemCinzaEColorido, 2000);
    mostrarImagem("Limiarizada (limiar=80)", limiarizada80, 2000);
    mostrarImagem("Limiarizada (limiar=128)", limiarizada128, 2000);
    mostrarImagem("Limiarizada (limiar=180)", limiarizada180, 2000);
    
    // Salva os resultados da limiarização
    cv::imwrite("../result/limiarizada_128_cinzaecolorido.jpg", limiarizada128);
    cv::imwrite("../result/limiarizada_80_cinzaecolorido.jpg", limiarizada80);
    cv::imwrite("../result/limiarizada_180_cinzaecolorido.jpg", limiarizada180);
    std::cout << "   - Limiarização (limiar=128) salva como result/limiarizada_128_cinzaecolorido.jpg" << std::endl;
    std::cout << "   - Limiarização (limiar=80) salva como result/limiarizada_80_cinzaecolorido.jpg" << std::endl;
    std::cout << "   - Limiarização (limiar=180) salva como result/limiarizada_180_cinzaecolorido.jpg" << std::endl;

    // =============================================================================
    // 5. ISOLAMENTO DE CANAIS - ANÁLISE DE COMPONENTES DE COR
    // =============================================================================
    std::cout << "\n5. Isolamento de canais (usando Colorida2)..." << std::endl;
    
    // Isola cada canal de cor individualmente usando a imagem já carregada
    cv::Mat canalAzul = Operation::isolateChannel(imagemColorida2, 0);     // Canal azul (B)
    cv::Mat canalVerde = Operation::isolateChannel(imagemColorida2, 1);    // Canal verde (G)
    cv::Mat canalVermelho = Operation::isolateChannel(imagemColorida2, 2); // Canal vermelho (R)
    std::cout << "Imagem Colorido2: " << imagemColorida2.cols << "x" << imagemColorida2.rows << " pixels, " << imagemColorida2.channels() << " canais" << std::endl;

    // Exibe cada componente de cor isolada
    mostrarImagem("Canal Azul Isolado", canalAzul, 2000);
    mostrarImagem("Canal Verde Isolado", canalVerde, 2000);
    mostrarImagem("Canal Vermelho Isolado", canalVermelho, 2000);
    
    // Salva os canais isolados
    cv::imwrite("../result/canal_azul.jpg", canalAzul);
    cv::imwrite("../result/canal_verde.jpg", canalVerde);
    cv::imwrite("../result/canal_vermelho.jpg", canalVermelho);
    std::cout << "   - Canal azul isolado salvo como result/canal_azul.jpg" << std::endl;
    std::cout << "   - Canal verde isolado salvo como result/canal_verde.jpg" << std::endl;
    std::cout << "   - Canal vermelho isolado salvo como result/canal_vermelho.jpg" << std::endl;

    // =============================================================================
    // 6. ANÁLISE DE HISTOGRAMA - COMPUTAÇÃO E VISUALIZAÇÃO
    // =============================================================================
    std::cout << "\n6. Computação e visualização de histograma..." << std::endl;
    
    // TESTE: Histograma da Colorido1
    std::cout << "\n   Analisando histograma da Colorido1..." << std::endl;
    std::vector<cv::Mat> histogramaColorido1 = Operation::computeHistogram(imagemColorida1);
    cv::Mat imagemHistogramaColorido1 = Operation::visualizeHistogram(histogramaColorido1);
    
    mostrarImagem("Colorido1 - Original", imagemColorida1, 3000);
    mostrarImagem("Colorido1 - Histograma", imagemHistogramaColorido1, 4000);
    
    cv::imwrite("../result/histograma_colorido1.jpg", imagemHistogramaColorido1);
    std::cout << "   - Histograma Colorido1 salvo como result/histograma_colorido1.jpg" << std::endl;
    
    // TESTE: Histograma da Colorido2
    std::cout << "\n   Analisando histograma da Colorido2..." << std::endl;
    std::vector<cv::Mat> histogramaColorido2 = Operation::computeHistogram(imagemColorida2);
    cv::Mat imagemHistogramaColorido2 = Operation::visualizeHistogram(histogramaColorido2);
    
    mostrarImagem("Colorido2 - Original", imagemColorida2, 3000);
    mostrarImagem("Colorido2 - Histograma", imagemHistogramaColorido2, 4000);
    
    cv::imwrite("../result/histograma_colorido2.jpg", imagemHistogramaColorido2);
    std::cout << "   - Histograma Colorido2 salvo como result/histograma_colorido2.jpg" << std::endl;

    // TESTE: Histograma da imagem CinzaEColorido
    std::cout << "\n   Analisando histograma da CinzaEColorido..." << std::endl;
    std::vector<cv::Mat> histogramaCinzaEColorido = Operation::computeHistogram(imagemCinzaEColorido);
    cv::Mat imagemHistogramaCinzaEColorido = Operation::visualizeHistogram(histogramaCinzaEColorido);
    
    mostrarImagem("CinzaEColorido - Histograma", imagemHistogramaCinzaEColorido, 4000);
    
    cv::imwrite("../result/histograma_cinzaecolorido.jpg", imagemHistogramaCinzaEColorido);
    std::cout << "   - Histograma CinzaEColorido salvo como result/histograma_cinzaecolorido.jpg" << std::endl;

    // =============================================================================
    // 7. INVERSÃO DE IMAGEM - EFEITO NEGATIVO
    // =============================================================================
    std::cout << "\n7. Inverso da imagem (Inverso)..." << std::endl;
    cv::Mat imagemInverso = cv::imread("../data/model/Inverso.jpeg");
    if (imagemInverso.empty()) {
        std::cerr << "Erro ao abrir ../data/model/Inverso.jpeg!" << std::endl;
        return 1;
    }
    
    std::cout << "Imagem Inverso: " << imagemInverso.cols << "x" << imagemInverso.rows << " pixels, " << imagemInverso.channels() << " canais" << std::endl;
    
    // Aplica inversão (efeito negativo)
    cv::Mat imagemInvertida = Operation::invert(imagemInverso);
    
    // Compara original vs invertida
    mostrarImagem("Original - Inverso", imagemInverso, 3000);
    mostrarImagem("Imagem Invertida", imagemInvertida, 3000);
    
    cv::imwrite("../result/imagem_invertida_inverso.jpg", imagemInvertida);
    std::cout << "   - Imagem invertida salva como result/imagem_invertida_inverso.jpg" << std::endl;

    // =============================================================================
    // FINALIZAÇÃO DO PROGRAMA
    // =============================================================================
    std::cout << "\n=== PROCESSAMENTO CONCLUÍDO ===" << std::endl;
    std::cout << "Todas as imagens foram salvas na pasta result/" << std::endl;
    std::cout << "Pressione qualquer tecla para fechar o programa..." << std::endl;
    
    // Aguarda tecla final e fecha todas as janelas
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}