#include <iostream>
#include <opencv2/opencv.hpp>
#include "OperacoesConvolucao.hpp"
#include "MorfologiaMatematica.hpp"
#include "DetectorBordas.hpp"
#include "ConversorTonsCinza.hpp"
#include <filesystem>

/**
 * EXIBIÇÃO DE IMAGENS
 * Redimensiona automaticamente imagens grandes para caber na tela
 */
void mostrarImagem(const std::string &nomeJanela, const cv::Mat &imagem)
{
    cv::Mat imagemRedimensionada;
    int larguraMax = 800;
    int alturaMax = 650;

    if (imagem.cols > larguraMax || imagem.rows > alturaMax)
    {
        double escala = std::min((double)larguraMax / imagem.cols, (double)alturaMax / imagem.rows);
        cv::resize(imagem, imagemRedimensionada, cv::Size(0, 0), escala, escala, cv::INTER_AREA);
    }
    else
    {
        imagemRedimensionada = imagem.clone();
    }

    cv::namedWindow(nomeJanela, cv::WINDOW_AUTOSIZE);
    cv::imshow(nomeJanela, imagemRedimensionada);
}

/**
 * PROGRAMA PRINCIPAL - M2.1
 * Implementação dos algoritmos de Operações no Domínio do Espaço
 */
int main()
{
    std::cout << "╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  TRABALHO M2.1 - OPERAÇÕES NO DOMÍNIO DO ESPAÇO       ║" << std::endl;
    std::cout << "║  Processamento Digital de Imagens - 2025              ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;

    // Cria pasta de resultados se não existir
    std::filesystem::create_directories("../data/result");

    // ======================================
    // 1. CONVOLUÇÃO SIMPLES
    // ======================================
    std::cout << "\n[1/3] Aplicando Convolução Simples..." << std::endl;
    
    cv::Mat imagemConv = cv::imread("../data/model/01_convolucao.png");
    if (imagemConv.empty()) {
        std::cerr << "   ✗ Erro: Não foi possível carregar 01_convolucao.png" << std::endl;
    } else {
        std::cout << "   ✓ Imagem carregada: 01_convolucao.png" << std::endl;
        
        // Converte para cinza
        cv::Mat imagemConvCinza = ConversorTonsCinza::paraMediaPonderada(imagemConv);
        
        // Cria diferentes kernels
        cv::Mat kernelPassaBaixa = OperacoesConvolucao::criarKernelPassaBaixa(3);
        cv::Mat kernelPassaAlta = OperacoesConvolucao::criarKernelPassaAlta(3);
        cv::Mat kernelNitidez = OperacoesConvolucao::criarKernelNitidez(3);
        
        std::cout << "   → Aplicando kernel Passa-Baixa (blur)..." << std::endl;
        cv::Mat convPassaBaixa = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelPassaBaixa);
        
        std::cout << "   → Aplicando kernel Passa-Alta..." << std::endl;
        cv::Mat convPassaAlta = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelPassaAlta);
        
        std::cout << "   → Aplicando kernel de Nitidez..." << std::endl;
        cv::Mat convNitidez = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelNitidez);
        
        // Exibe resultados
        mostrarImagem("Convolução - Original", imagemConvCinza);
        mostrarImagem("Convolução - Passa-Baixa (Blur)", convPassaBaixa);
        mostrarImagem("Convolução - Passa-Alta", convPassaAlta);
        mostrarImagem("Convolução - Nitidez", convNitidez);
        
        // Salva resultados
        cv::imwrite("../data/result/01_convolucao_original.png", imagemConvCinza);
        cv::imwrite("../data/result/01_convolucao_passa_baixa.png", convPassaBaixa);
        cv::imwrite("../data/result/01_convolucao_passa_alta.png", convPassaAlta);
        cv::imwrite("../data/result/01_convolucao_nitidez.png", convNitidez);
        
        std::cout << "   ✓ Convolução concluída - 4 imagens salvas" << std::endl;
    }

    // ======================================
    // 2. MORFOLOGIA MATEMÁTICA
    // ======================================
    std::cout << "\n[2/3] Aplicando Morfologia Matemática..." << std::endl;
    
    // Elemento estruturante padrão 3x3
    cv::Mat elementoEstruturante = MorfologiaMatematica::criarElementoEstruturante(3);
    
    // 2.1 Erosão
    cv::Mat imagemErosao = cv::imread("../data/model/02_erosao.png", cv::IMREAD_GRAYSCALE);
    if (!imagemErosao.empty()) {
        std::cout << "   → Erosão..." << std::endl;
        cv::Mat resultErosao = MorfologiaMatematica::erosao(imagemErosao, elementoEstruturante);
        mostrarImagem("Morfologia - Erosão Original", imagemErosao);
        mostrarImagem("Morfologia - Erosão Resultado", resultErosao);
        cv::imwrite("../data/result/02_erosao_original.png", imagemErosao);
        cv::imwrite("../data/result/02_erosao_resultado.png", resultErosao);
        std::cout << "   ✓ Erosão concluída" << std::endl;
    }
    
    // 2.2 Dilatação
    cv::Mat imagemDilatacao = cv::imread("../data/model/03_dilatacao.png", cv::IMREAD_GRAYSCALE);
    if (!imagemDilatacao.empty()) {
        std::cout << "   → Dilatação..." << std::endl;
        cv::Mat resultDilatacao = MorfologiaMatematica::dilatacao(imagemDilatacao, elementoEstruturante);
        mostrarImagem("Morfologia - Dilatação Original", imagemDilatacao);
        mostrarImagem("Morfologia - Dilatação Resultado", resultDilatacao);
        cv::imwrite("../data/result/03_dilatacao_original.png", imagemDilatacao);
        cv::imwrite("../data/result/03_dilatacao_resultado.png", resultDilatacao);
        std::cout << "   ✓ Dilatação concluída" << std::endl;
    }
    
    // 2.3 Abertura
    cv::Mat imagemAbertura = cv::imread("../data/model/04_abertura.png", cv::IMREAD_GRAYSCALE);
    if (!imagemAbertura.empty()) {
        std::cout << "   → Abertura (Erosão + Dilatação)..." << std::endl;
        cv::Mat resultAbertura = MorfologiaMatematica::abertura(imagemAbertura, elementoEstruturante);
        mostrarImagem("Morfologia - Abertura Original", imagemAbertura);
        mostrarImagem("Morfologia - Abertura Resultado", resultAbertura);
        cv::imwrite("../data/result/04_abertura_original.png", imagemAbertura);
        cv::imwrite("../data/result/04_abertura_resultado.png", resultAbertura);
        std::cout << "   ✓ Abertura concluída" << std::endl;
    }
    
    // 2.4 Fechamento
    cv::Mat imagemFechamento = cv::imread("../data/model/05_fechamento.png", cv::IMREAD_GRAYSCALE);
    if (!imagemFechamento.empty()) {
        std::cout << "   → Fechamento (Dilatação + Erosão)..." << std::endl;
        cv::Mat resultFechamento = MorfologiaMatematica::fechamento(imagemFechamento, elementoEstruturante);
        mostrarImagem("Morfologia - Fechamento Original", imagemFechamento);
        mostrarImagem("Morfologia - Fechamento Resultado", resultFechamento);
        cv::imwrite("../data/result/05_fechamento_original.png", imagemFechamento);
        cv::imwrite("../data/result/05_fechamento_resultado.png", resultFechamento);
        std::cout << "   ✓ Fechamento concluído" << std::endl;
    }
    
    // 2.5 Limites (Interno e Externo)
    cv::Mat imagemLimites = cv::imread("../data/model/06_limites.png", cv::IMREAD_GRAYSCALE);
    if (!imagemLimites.empty()) {
        std::cout << "   → Limites Interno e Externo..." << std::endl;
        cv::Mat resultLimiteInterno = MorfologiaMatematica::limiteInterno(imagemLimites, elementoEstruturante);
        cv::Mat resultLimiteExterno = MorfologiaMatematica::limiteExterno(imagemLimites, elementoEstruturante);
        mostrarImagem("Morfologia - Limites Original", imagemLimites);
        mostrarImagem("Morfologia - Limite Interno", resultLimiteInterno);
        mostrarImagem("Morfologia - Limite Externo", resultLimiteExterno);
        cv::imwrite("../data/result/06_limites_original.png", imagemLimites);
        cv::imwrite("../data/result/06_limites_interno.png", resultLimiteInterno);
        cv::imwrite("../data/result/06_limites_externo.png", resultLimiteExterno);
        std::cout << "   ✓ Limites concluídos" << std::endl;
    }
    
    std::cout << "   ✓ Morfologia Matemática concluída" << std::endl;

    // ======================================
    // 3. IDENTIFICAÇÃO DE BORDAS
    // ======================================
    std::cout << "\n[3/3] Aplicando Detecção de Bordas..." << std::endl;
    
    cv::Mat imagemBordas = cv::imread("../data/model/07_bordas.png");
    if (imagemBordas.empty()) {
        std::cerr << "   ✗ Erro: Não foi possível carregar 07_bordas.png" << std::endl;
    } else {
        std::cout << "   ✓ Imagem carregada: 07_bordas.png" << std::endl;
        
        // Converte para cinza
        cv::Mat imagemBordasCinza = ConversorTonsCinza::paraMediaPonderada(imagemBordas);
        
        // Roberts
        std::cout << "   → Operador de Roberts..." << std::endl;
        cv::Mat bordasRoberts = DetectorBordas::roberts(imagemBordasCinza);
        cv::Mat bordasRobertsLimiar = DetectorBordas::aplicarLimiar(bordasRoberts, 50);
        
        // Sobel
        std::cout << "   → Operador de Sobel..." << std::endl;
        cv::Mat bordasSobel = DetectorBordas::sobel(imagemBordasCinza);
        cv::Mat bordasSobelLimiar = DetectorBordas::aplicarLimiar(bordasSobel, 50);
        
        // Robinson
        std::cout << "   → Operador de Robinson..." << std::endl;
        cv::Mat bordasRobinson = DetectorBordas::robinson(imagemBordasCinza);
        cv::Mat bordasRobinsonLimiar = DetectorBordas::aplicarLimiar(bordasRobinson, 50);
        
        // Exibe resultados
        mostrarImagem("Bordas - Original", imagemBordasCinza);
        mostrarImagem("Bordas - Roberts", bordasRoberts);
        mostrarImagem("Bordas - Roberts Limiarizado", bordasRobertsLimiar);
        mostrarImagem("Bordas - Sobel", bordasSobel);
        mostrarImagem("Bordas - Sobel Limiarizado", bordasSobelLimiar);
        mostrarImagem("Bordas - Robinson", bordasRobinson);
        mostrarImagem("Bordas - Robinson Limiarizado", bordasRobinsonLimiar);
        
        // Salva resultados
        cv::imwrite("../data/result/07_bordas_original.png", imagemBordasCinza);
        cv::imwrite("../data/result/07_bordas_roberts.png", bordasRoberts);
        cv::imwrite("../data/result/07_bordas_roberts_limiar.png", bordasRobertsLimiar);
        cv::imwrite("../data/result/07_bordas_sobel.png", bordasSobel);
        cv::imwrite("../data/result/07_bordas_sobel_limiar.png", bordasSobelLimiar);
        cv::imwrite("../data/result/07_bordas_robinson.png", bordasRobinson);
        cv::imwrite("../data/result/07_bordas_robinson_limiar.png", bordasRobinsonLimiar);
        
        std::cout << "   ✓ Detecção de Bordas concluída - 7 imagens salvas" << std::endl;
    }

    // FINALIZAÇÃO
    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  ✓ PROCESSAMENTO M2.1 CONCLUÍDO COM SUCESSO!          ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\nResumo dos Algoritmos Implementados:" << std::endl;
    std::cout << "  1. Convolução Simples" << std::endl;
    std::cout << "     • Kernel Passa-Baixa (blur/suavização)" << std::endl;
    std::cout << "     • Kernel Passa-Alta (realce de bordas)" << std::endl;
    std::cout << "     • Kernel de Nitidez (sharpening)" << std::endl;
    std::cout << "\n  2. Morfologia Matemática" << std::endl;
    std::cout << "     • Erosão" << std::endl;
    std::cout << "     • Dilatação" << std::endl;
    std::cout << "     • Abertura (remove ruídos pequenos)" << std::endl;
    std::cout << "     • Fechamento (preenche buracos pequenos)" << std::endl;
    std::cout << "     • Limites Interno e Externo" << std::endl;
    std::cout << "\n  3. Detecção de Bordas" << std::endl;
    std::cout << "     • Operador de Roberts (2x2)" << std::endl;
    std::cout << "     • Operador de Sobel (3x3)" << std::endl;
    std::cout << "     • Operador de Robinson (8 direções)" << std::endl;
    std::cout << "\n📁 Resultados salvos em: data/result/" << std::endl;
    std::cout << "\n⌨️  Pressione qualquer tecla para fechar as janelas..." << std::endl;

    // Aguarda tecla e fecha janelas
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    return 0;
}
