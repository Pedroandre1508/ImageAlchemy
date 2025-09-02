#include <iostream>
#include <opencv2/opencv.hpp>
#include "ProcessadorImagens.hpp"
#include "ProcessadorHistogramas.hpp"
#include "OperacoesAritmeticas.hpp"
#include "ConversorTonsCinza.hpp"
#include <filesystem>

/**
 * EXIBIÇÃO DE IMAGENS
 *
 * Funcionamento:
 * - Redimensiona automaticamente imagens grandes para caber na tela
 * - Mantém proporção original da imagem
 * - Cria janela com nome personalizado
 * - Controla tempo de exibição
 */
void mostrarImagem(const std::string &nomeJanela, const cv::Mat &imagem)
{
    // Configurações de tamanho máximo para as janelas
    cv::Mat imagemRedimensionada;
    int larguraMax = 800;
    int alturaMax = 650;

    // Verifica se a imagem é maior que o tamanho máximo permitido
    if (imagem.cols > larguraMax || imagem.rows > alturaMax)
    {
        // Calcula o fator de escala mantendo a proporção
        double escala = std::min((double)larguraMax / imagem.cols, (double)alturaMax / imagem.rows);
        // Redimensiona usando interpolação bilinear (boa qualidade)
        cv::resize(imagem, imagemRedimensionada, cv::Size(0, 0), escala, escala, cv::INTER_AREA);
    }
    else
    {
        // Se a imagem já é pequena, usa o tamanho original
        imagemRedimensionada = imagem.clone();
    }

    // Cria janela com tamanho automático baseado na imagem
    cv::namedWindow(nomeJanela, cv::WINDOW_AUTOSIZE);
    cv::imshow(nomeJanela, imagemRedimensionada);
}

/**
 * PROGRAMA PRINCIPAL
 *
 * Estrutura:
 * 1. Conversões para tons de cinza
 * 2. Operações aritméticas com escalares
 * 3. Operações aritméticas entre imagens
 * 4. Limiarização
 * 5. Isolamento de canais de cor
 * 6. Histograma
 * 7. Inversão de imagem
 */
int main()
{

    // Cria pasta de resultados se não existir
    std::filesystem::create_directories("../data/result");

    // CONVERSÃO PARA TONS DE CINZA
    cv::Mat imagemColorida1 = cv::imread("../data/model/Colorida1.jpeg");
    if (imagemColorida1.empty())
    {
        std::cerr << "Erro ao abrir ../data/model/Colorida1.jpeg!" << std::endl;
        return 1;
    }

    // Carrega a segunda imagem colorida para operações entre imagens
    cv::Mat imagemColorida2 = cv::imread("../data/model/Colorida2.jpeg");
    if (imagemColorida2.empty())
    {
        std::cerr << "Erro ao abrir ../data/model/Colorida2.jpeg!" << std::endl;
        return 1;
    }

    // Exibe a imagem original
    mostrarImagem("Original - Colorida1", imagemColorida1);

    // Aplica as duas técnicas de médias de conversão para cinza usando ConversorTonsCinza
    cv::Mat cinzaMediaAritmetica = ConversorTonsCinza::paraMediaAritmetica(imagemColorida1);
    cv::Mat cinzaMediaPonderada = ConversorTonsCinza::paraMediaPonderada(imagemColorida1);

    // Exibe os resultados
    mostrarImagem("Conversão - Média Aritmética", cinzaMediaAritmetica);
    mostrarImagem("Conversão - Média Ponderada", cinzaMediaPonderada);

    // Salva os resultados
    cv::imwrite("../data/result/cinza_media_aritmetica_colorido1.jpg", cinzaMediaAritmetica);
    cv::imwrite("../data/result/cinza_media_ponderada_colorido1.jpg", cinzaMediaPonderada);

    // OPERAÇÕES ARITMÉTICAS COM ESCALAR usando OperacoesAritmeticas
    // Aplica diferentes operações para demonstrar efeitos
    cv::Mat imagemClareada = OperacoesAritmeticas::somarEscalar(imagemColorida1, 50);        // +50: Clareia
    cv::Mat imagemEscurecida = OperacoesAritmeticas::subtrairEscalar(imagemColorida1, 30); // -30: Escurece
    cv::Mat imagemContraste = OperacoesAritmeticas::multiplicarEscalar(imagemColorida1, 1.5); // *1.5: Aumenta contraste
    cv::Mat imagemReduzida = OperacoesAritmeticas::dividirEscalar(imagemColorida1, 2.0);    // /2: Reduz intensidade

    // Exibe sequência comparativa
    mostrarImagem("Original", imagemColorida1);
    mostrarImagem("Clareada (+50)", imagemClareada);
    mostrarImagem("Escurecida (-30)", imagemEscurecida);
    mostrarImagem("Contraste (*1.5)", imagemContraste);
    mostrarImagem("Reduzida (/2)", imagemReduzida);

    // Salva todos os resultados
    cv::imwrite("../data/result/imagem_clareada.jpg", imagemClareada);
    cv::imwrite("../data/result/imagem_escurecida.jpg", imagemEscurecida);
    cv::imwrite("../data/result/imagem_contraste.jpg", imagemContraste);
    cv::imwrite("../data/result/imagem_reduzida.jpg", imagemReduzida);

    // 3. OPERAÇÕES ARITMÉTICAS ENTRE IMAGENS usando OperacoesAritmeticas

    // Operações entre duas imagens coloridas diferentes
    cv::Mat somaCores = OperacoesAritmeticas::somarImagens(imagemColorida1, imagemColorida2);
    cv::Mat subtracaoCores = OperacoesAritmeticas::subtrairImagens(imagemColorida1, imagemColorida2);
    cv::Mat multiplicacaoCores = OperacoesAritmeticas::multiplicarImagens(imagemColorida1, imagemColorida2);
    cv::Mat divisaoCores = OperacoesAritmeticas::dividirImagens(imagemColorida1, imagemColorida2);

    // Exibe operações
    mostrarImagem("Colorida1 + Colorida2", somaCores);
    mostrarImagem("Colorida1 - Colorida2", subtracaoCores);
    mostrarImagem("Colorida1 * Colorida2", multiplicacaoCores);
    mostrarImagem("Colorida1 / Colorida2", divisaoCores);

    // Salva operações
    cv::imwrite("../data/result/soma_colorida1_colorida2.jpg", somaCores);
    cv::imwrite("../data/result/subtracao_colorida1_colorida2.jpg", subtracaoCores);
    cv::imwrite("../data/result/multiplicacao_colorida1_colorida2.jpg", multiplicacaoCores);
    cv::imwrite("../data/result/divisao_colorida1_colorida2.jpg", divisaoCores);

    // Converte Colorida1 para tons de cinza real usando ConversorTonsCinza
    cv::Mat cinzaReal = ConversorTonsCinza::paraMediaPonderada(imagemColorida1);

    // Demonstra operações entre imagem colorida e imagem em tons de cinza
    cv::Mat somaColoridaCinza = OperacoesAritmeticas::somarImagens(imagemColorida1, cinzaReal);
    cv::Mat subtracaoColoridaCinza = OperacoesAritmeticas::subtrairImagens(imagemColorida1, cinzaReal);
    cv::Mat multiplicacaoColoridaCinza = OperacoesAritmeticas::multiplicarImagens(imagemColorida1, cinzaReal);
    cv::Mat divisaoColoridaCinza = OperacoesAritmeticas::dividirImagens(imagemColorida1, cinzaReal);

    // Exibe operações
    mostrarImagem("Cinza Real (convertido)", cinzaReal);
    mostrarImagem("Colorida + Cinza", somaColoridaCinza);
    mostrarImagem("Colorida - Cinza", subtracaoColoridaCinza);
    mostrarImagem("Colorida * Cinza", multiplicacaoColoridaCinza);
    mostrarImagem("Colorida / Cinza", divisaoColoridaCinza);

    // Salva operações
    cv::imwrite("../data/result/cinza_real_convertido.jpg", cinzaReal);
    cv::imwrite("../data/result/soma_colorida_cinza.jpg", somaColoridaCinza);
    cv::imwrite("../data/result/subtracao_colorida_cinza.jpg", subtracaoColoridaCinza);
    cv::imwrite("../data/result/multiplicacao_colorida_cinza.jpg", multiplicacaoColoridaCinza);
    cv::imwrite("../data/result/divisao_colorida_cinza.jpg", divisaoColoridaCinza);

    // 4. LIMIARIZAÇÃO usando ProcessadorImagens
    cv::Mat imagemCinzaEColorido = cv::imread("../data/model/CinzaEColorido.jpeg");
    if (imagemCinzaEColorido.empty())
    {
        std::cerr << "Erro ao abrir ../data/model/CinzaEColorido.jpeg!" << std::endl;
        return 1;
    }

    // Aplica diferentes valores de limiar para demonstrar o efeito
    cv::Mat limiarizada128 = ProcessadorImagens::aplicarLimiarizacao(imagemCinzaEColorido, 128, 255); // Limiar médio
    cv::Mat limiarizada80 = ProcessadorImagens::aplicarLimiarizacao(imagemCinzaEColorido, 80, 255);   // Limiar baixo (mais branco)
    cv::Mat limiarizada180 = ProcessadorImagens::aplicarLimiarizacao(imagemCinzaEColorido, 180, 255); // Limiar alto (mais preto)

    // Mostra os diferentes limiares
    mostrarImagem("Original - CinzaEColorido", imagemCinzaEColorido);
    mostrarImagem("Limiarizada (limiar=80)", limiarizada80);
    mostrarImagem("Limiarizada (limiar=128)", limiarizada128);
    mostrarImagem("Limiarizada (limiar=180)", limiarizada180);

    // Salva os resultados
    cv::imwrite("../data/result/limiarizada_128_cinzaecolorido.jpg", limiarizada128);
    cv::imwrite("../data/result/limiarizada_80_cinzaecolorido.jpg", limiarizada80);
    cv::imwrite("../data/result/limiarizada_180_cinzaecolorido.jpg", limiarizada180);

    // 5. ISOLAMENTO DE CANAIS - ANÁLISE DE COMPONENTES DE COR usando ProcessadorImagens
    // Isola cada canal de cor individualmente usando a imagem já carregada
    cv::Mat canalAzul = ProcessadorImagens::isolarCanal(imagemColorida2, 0);     // Canal azul (B)
    cv::Mat canalVerde = ProcessadorImagens::isolarCanal(imagemColorida2, 1);    // Canal verde (G)
    cv::Mat canalVermelho = ProcessadorImagens::isolarCanal(imagemColorida2, 2); // Canal vermelho (R)

    // Exibe cada componente de cor isolada
    mostrarImagem("Canal Azul Isolado", canalAzul);
    mostrarImagem("Canal Verde Isolado", canalVerde);
    mostrarImagem("Canal Vermelho Isolado", canalVermelho);

    // Salva os canais isolados
    cv::imwrite("../data/result/canal_azul.jpg", canalAzul);
    cv::imwrite("../data/result/canal_verde.jpg", canalVerde);
    cv::imwrite("../data/result/canal_vermelho.jpg", canalVermelho);

    // 6. ANÁLISE DE HISTOGRAMA - COMPUTAÇÃO E VISUALIZAÇÃO usando ProcessadorHistogramas

    // TESTE: Histograma da imagemColorido1
    std::vector<cv::Mat> histogramaColorido1 = ProcessadorHistogramas::calcularHistograma(imagemColorida1);
    cv::Mat imagemHistogramaColorido1 = ProcessadorHistogramas::visualizarHistograma(histogramaColorido1);

    mostrarImagem("Colorido1 - Original", imagemColorida1);
    mostrarImagem("Colorido1 - Histograma", imagemHistogramaColorido1);

    // Salva Histograma
    cv::imwrite("../data/result/histograma_colorido1.jpg", imagemHistogramaColorido1);

    // TESTE: Histograma da imagem Colorido2
    std::vector<cv::Mat> histogramaColorido2 = ProcessadorHistogramas::calcularHistograma(imagemColorida2);
    cv::Mat imagemHistogramaColorido2 = ProcessadorHistogramas::visualizarHistograma(histogramaColorido2);

    mostrarImagem("Colorido2 - Original", imagemColorida2);
    mostrarImagem("Colorido2 - Histograma", imagemHistogramaColorido2);

    // Salva Histograma
    cv::imwrite("../data/result/histograma_colorido2.jpg", imagemHistogramaColorido2);

    // TESTE: Histograma da imagem CinzaEColorido
    std::vector<cv::Mat> histogramaCinzaEColorido = ProcessadorHistogramas::calcularHistograma(imagemCinzaEColorido);
    cv::Mat imagemHistogramaCinzaEColorido = ProcessadorHistogramas::visualizarHistograma(histogramaCinzaEColorido);

    // mostra imagem
    mostrarImagem("CinzaEColorido - Histograma", imagemHistogramaCinzaEColorido);

    // Salva Histograma
    cv::imwrite("../data/result/histograma_cinzaecolorido.jpg", imagemHistogramaCinzaEColorido);

    // 7. INVERSÃO DE IMAGEM usando ProcessadorImagens
    cv::Mat imagemInverso = cv::imread("../data/model/Inverso.jpeg");
    if (imagemInverso.empty())
    {
        std::cerr << "Erro ao abrir ../data/model/Inverso.jpeg!" << std::endl;
        return 1;
    }

    // Aplica inversão (efeito negativo)
    cv::Mat imagemInvertida = ProcessadorImagens::inverterImagem(imagemInverso);

    // Exibe o invertida
    mostrarImagem("Original - Inverso", imagemInverso);
    mostrarImagem("Imagem Invertida", imagemInvertida);

    // Salva
    cv::imwrite("../data/result/imagem_invertida_inverso.jpg", imagemInvertida);

    // FINALIZAÇÃO DO PROGRAMA
    std::cout << "\n=== PROCESSAMENTO CONCLUÍDO ===" << std::endl;
    std::cout << "Todas as imagens foram salvas na pasta result/" << std::endl;

    // Aguarda tecla final e fecha todas as janelas
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    return 0;
}