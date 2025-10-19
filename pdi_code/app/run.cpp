#include <iostream>
#include <opencv2/opencv.hpp>
#include "ProcessadorImagens.hpp"
#include "ProcessadorHistogramas.hpp"
#include "OperacoesAritmeticas.hpp"
#include "ConversorTonsCinza.hpp"
#include "OperacoesConvolucao.hpp"
#include "MorfologiaMatematica.hpp"
#include "DetectorBordas.hpp"
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

    // ======================================
    // M2.1 - OPERAÇÕES NO DOMÍNIO DO ESPAÇO
    // ======================================
    
    std::cout << "\n=== INICIANDO PROCESSAMENTO M2.1 ===" << std::endl;
    
    // 1. CONVOLUÇÃO SIMPLES (Imagens em tons de cinza)
    std::cout << "\n1. Aplicando Convolução..." << std::endl;
    
    // Carrega imagem de teste (usa as imagens do modelo)
    cv::Mat imagemConv = cv::imread("../data/model/01_convolucao.png");
    if (imagemConv.empty()) {
        std::cerr << "Aviso: ../data/model/01_convolucao.png não encontrada. Usando imagem alternativa." << std::endl;
        imagemConv = cinzaReal;
    }
    
    // Converte para cinza
    cv::Mat imagemConvCinza = ConversorTonsCinza::paraMediaPonderada(imagemConv);
    
    // Testa diferentes kernels
    cv::Mat kernelPassaBaixa = OperacoesConvolucao::criarKernelPassaBaixa(3);
    cv::Mat kernelPassaAlta = OperacoesConvolucao::criarKernelPassaAlta(3);
    cv::Mat kernelNitidez = OperacoesConvolucao::criarKernelNitidez(3);
    
    cv::Mat convPassaBaixa = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelPassaBaixa);
    cv::Mat convPassaAlta = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelPassaAlta);
    cv::Mat convNitidez = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelNitidez);
    
    mostrarImagem("Convolução - Original", imagemConvCinza);
    mostrarImagem("Convolução - Passa-Baixa (Blur)", convPassaBaixa);
    mostrarImagem("Convolução - Passa-Alta", convPassaAlta);
    mostrarImagem("Convolução - Nitidez", convNitidez);
    
    cv::imwrite("../data/result/01_convolucao_original.png", imagemConvCinza);
    cv::imwrite("../data/result/01_convolucao_passa_baixa.png", convPassaBaixa);
    cv::imwrite("../data/result/01_convolucao_passa_alta.png", convPassaAlta);
    cv::imwrite("../data/result/01_convolucao_nitidez.png", convNitidez);
    
    // 2. MORFOLOGIA MATEMÁTICA (Imagens binárias)
    std::cout << "2. Aplicando Morfologia Matemática..." << std::endl;
    
    // Carrega imagens de teste
    cv::Mat imagemErosao = cv::imread("../data/model/02_erosao.png", cv::IMREAD_GRAYSCALE);
    cv::Mat imagemDilatacao = cv::imread("../data/model/03_dilatacao.png", cv::IMREAD_GRAYSCALE);
    cv::Mat imagemAbertura = cv::imread("../data/model/04_abertura.png", cv::IMREAD_GRAYSCALE);
    cv::Mat imagemFechamento = cv::imread("../data/model/05_fechamento.png", cv::IMREAD_GRAYSCALE);
    cv::Mat imagemLimites = cv::imread("../data/model/06_limites.png", cv::IMREAD_GRAYSCALE);
    
    // Se não encontrar as imagens, usa a imagem limiarizada como base
    if (imagemErosao.empty()) imagemErosao = limiarizada128;
    if (imagemDilatacao.empty()) imagemDilatacao = limiarizada128;
    if (imagemAbertura.empty()) imagemAbertura = limiarizada128;
    if (imagemFechamento.empty()) imagemFechamento = limiarizada128;
    if (imagemLimites.empty()) imagemLimites = limiarizada128;
    
    // Cria elemento estruturante
    cv::Mat elementoEstruturante = MorfologiaMatematica::criarElementoEstruturante(3);
    cv::Mat elementoEstruturanteCruz = MorfologiaMatematica::criarElementoEstruturanteCruz(5);
    
    // Erosão
    cv::Mat resultErosao = MorfologiaMatematica::erosao(imagemErosao, elementoEstruturante);
    mostrarImagem("Morfologia - Erosão Original", imagemErosao);
    mostrarImagem("Morfologia - Erosão Resultado", resultErosao);
    cv::imwrite("../data/result/02_erosao_original.png", imagemErosao);
    cv::imwrite("../data/result/02_erosao_resultado.png", resultErosao);
    
    // Dilatação
    cv::Mat resultDilatacao = MorfologiaMatematica::dilatacao(imagemDilatacao, elementoEstruturante);
    mostrarImagem("Morfologia - Dilatação Original", imagemDilatacao);
    mostrarImagem("Morfologia - Dilatação Resultado", resultDilatacao);
    cv::imwrite("../data/result/03_dilatacao_original.png", imagemDilatacao);
    cv::imwrite("../data/result/03_dilatacao_resultado.png", resultDilatacao);
    
    // Abertura
    cv::Mat resultAbertura = MorfologiaMatematica::abertura(imagemAbertura, elementoEstruturante);
    mostrarImagem("Morfologia - Abertura Original", imagemAbertura);
    mostrarImagem("Morfologia - Abertura Resultado", resultAbertura);
    cv::imwrite("../data/result/04_abertura_original.png", imagemAbertura);
    cv::imwrite("../data/result/04_abertura_resultado.png", resultAbertura);
    
    // Fechamento
    cv::Mat resultFechamento = MorfologiaMatematica::fechamento(imagemFechamento, elementoEstruturante);
    mostrarImagem("Morfologia - Fechamento Original", imagemFechamento);
    mostrarImagem("Morfologia - Fechamento Resultado", resultFechamento);
    cv::imwrite("../data/result/05_fechamento_original.png", imagemFechamento);
    cv::imwrite("../data/result/05_fechamento_resultado.png", resultFechamento);
    
    // Limites (Interno e Externo)
    cv::Mat resultLimiteInterno = MorfologiaMatematica::limiteInterno(imagemLimites, elementoEstruturante);
    cv::Mat resultLimiteExterno = MorfologiaMatematica::limiteExterno(imagemLimites, elementoEstruturante);
    mostrarImagem("Morfologia - Limites Original", imagemLimites);
    mostrarImagem("Morfologia - Limite Interno", resultLimiteInterno);
    mostrarImagem("Morfologia - Limite Externo", resultLimiteExterno);
    cv::imwrite("../data/result/06_limites_original.png", imagemLimites);
    cv::imwrite("../data/result/06_limites_interno.png", resultLimiteInterno);
    cv::imwrite("../data/result/06_limites_externo.png", resultLimiteExterno);
    
    // 3. IDENTIFICAÇÃO DE BORDAS (Imagens em tons de cinza)
    std::cout << "3. Aplicando Detecção de Bordas..." << std::endl;
    
    // Carrega imagem de teste
    cv::Mat imagemBordas = cv::imread("../data/model/07_bordas.png");
    if (imagemBordas.empty()) {
        std::cerr << "Aviso: ../data/model/07_bordas.png não encontrada. Usando imagem alternativa." << std::endl;
        imagemBordas = imagemCinzaEColorido;
    }
    
    cv::Mat imagemBordasCinza = ConversorTonsCinza::paraMediaPonderada(imagemBordas);
    
    // Roberts
    cv::Mat bordasRoberts = DetectorBordas::roberts(imagemBordasCinza);
    cv::Mat bordasRobertsLimiar = DetectorBordas::aplicarLimiar(bordasRoberts, 50);
    mostrarImagem("Bordas - Roberts", bordasRoberts);
    mostrarImagem("Bordas - Roberts Limiarizado", bordasRobertsLimiar);
    cv::imwrite("../data/result/07_bordas_roberts.png", bordasRoberts);
    cv::imwrite("../data/result/07_bordas_roberts_limiar.png", bordasRobertsLimiar);
    
    // Sobel
    cv::Mat bordasSobel = DetectorBordas::sobel(imagemBordasCinza);
    cv::Mat bordasSobelLimiar = DetectorBordas::aplicarLimiar(bordasSobel, 50);
    mostrarImagem("Bordas - Sobel", bordasSobel);
    mostrarImagem("Bordas - Sobel Limiarizado", bordasSobelLimiar);
    cv::imwrite("../data/result/07_bordas_sobel.png", bordasSobel);
    cv::imwrite("../data/result/07_bordas_sobel_limiar.png", bordasSobelLimiar);
    
    // Robinson
    cv::Mat bordasRobinson = DetectorBordas::robinson(imagemBordasCinza);
    cv::Mat bordasRobinsonLimiar = DetectorBordas::aplicarLimiar(bordasRobinson, 50);
    mostrarImagem("Bordas - Robinson", bordasRobinson);
    mostrarImagem("Bordas - Robinson Limiarizado", bordasRobinsonLimiar);
    cv::imwrite("../data/result/07_bordas_robinson.png", bordasRobinson);
    cv::imwrite("../data/result/07_bordas_robinson_limiar.png", bordasRobinsonLimiar);
    
    // Comparação lado a lado
    mostrarImagem("Bordas - Original", imagemBordasCinza);
    cv::imwrite("../data/result/07_bordas_original.png", imagemBordasCinza);

    // FINALIZAÇÃO DO PROGRAMA
    std::cout << "\n=== PROCESSAMENTO M2.1 CONCLUÍDO ===" << std::endl;
    std::cout << "Todas as imagens foram salvas na pasta result/" << std::endl;
    std::cout << "\nAlgoritmos implementados:" << std::endl;
    std::cout << "1. Convolução Simples (passa-baixa, passa-alta, nitidez)" << std::endl;
    std::cout << "2. Morfologia Matemática (erosão, dilatação, abertura, fechamento, limites)" << std::endl;
    std::cout << "3. Detecção de Bordas (Roberts, Sobel, Robinson)" << std::endl;

    // Aguarda tecla final e fecha todas as janelas
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    return 0;
}