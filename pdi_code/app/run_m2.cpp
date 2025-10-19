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
    std::cout << "TRABALHO M2.1 - OPERAÇÕES NO DOMÍNIO DO ESPAÇO" << std::endl;
    std::cout << "Processamento Digital de Imagens - 2025" << std::endl;

    // Cria pastas de resultados organizadas
    std::filesystem::create_directories("../data/result/Convolução Simples");
    std::filesystem::create_directories("../data/result/Morfologia Matemática");
    std::filesystem::create_directories("../data/result/Identificação de Bordas");

    // 1. CONVOLUÇÃO SIMPLES    
    std::vector<std::string> imagensConvolucao = {
        "01_convolucao.png", "Cinza1.jpeg", "Cinza2.jpeg", "Cinza3.jpeg"
    };
    
    // Cria diferentes kernels
    cv::Mat kernelPassaBaixa = OperacoesConvolucao::criarKernelPassaBaixa(3);
    cv::Mat kernelPassaAlta = OperacoesConvolucao::criarKernelPassaAlta(3);
    cv::Mat kernelNitidez = OperacoesConvolucao::criarKernelNitidez(3);
    
    for (size_t i = 0; i < imagensConvolucao.size(); i++) {
        std::string nomeArquivo = imagensConvolucao[i];
        cv::Mat imagemConv = cv::imread("../data/model/" + nomeArquivo);
        
        if (imagemConv.empty()) {
            std::cerr << "   ✗ Erro ao carregar: " << nomeArquivo << std::endl;
            continue;
        }
                
        // Converte para cinza
        cv::Mat imagemConvCinza = ConversorTonsCinza::paraMediaPonderada(imagemConv);
        
        // Aplica convoluções
        cv::Mat convPassaBaixa = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelPassaBaixa);
        cv::Mat convPassaAlta = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelPassaAlta);
        cv::Mat convNitidez = OperacoesConvolucao::aplicarConvolucao(imagemConvCinza, kernelNitidez);
        
        // Salva resultados na pasta organizada
        std::string prefixo = "../data/result/Convolução Simples/conv_";
        if (nomeArquivo.find("01_convolucao") != std::string::npos) {
            prefixo += "01_";
        } else if (nomeArquivo.find("Cinza1") != std::string::npos) {
            prefixo += "Cinza1_";
        } else if (nomeArquivo.find("Cinza2") != std::string::npos) {
            prefixo += "Cinza2_";
        } else if (nomeArquivo.find("Cinza3") != std::string::npos) {
            prefixo += "Cinza3_";
        }
        
        cv::imwrite(prefixo + "original.png", imagemConvCinza);
        cv::imwrite(prefixo + "passa_baixa.png", convPassaBaixa);
        cv::imwrite(prefixo + "passa_alta.png", convPassaAlta);
        cv::imwrite(prefixo + "nitidez.png", convNitidez);
        
        // Exibe as imagens processadas
        mostrarImagem("Original - " + nomeArquivo, imagemConvCinza);
        mostrarImagem("Passa-Baixa - " + nomeArquivo, convPassaBaixa);
        mostrarImagem("Passa-Alta - " + nomeArquivo, convPassaAlta);
        mostrarImagem("Nitidez - " + nomeArquivo, convNitidez);
        
        std::cout << "     ✓ " << nomeArquivo << " processada (4 resultados)" << std::endl;
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    
    // 2. MORFOLOGIA MATEMÁTICA
    // Lista de imagens binárias: Binario1, Binario2, Binario3
    std::vector<std::string> imagensBinarias = {
        "Binario1.jpg", "Binario2.jpeg", "Binario3.jpg"
    };
    
    // Elemento estruturante padrão 3x3
    cv::Mat elementoEstruturante = MorfologiaMatematica::criarElementoEstruturante(3);
    
    // Processa cada operação morfológica com todas as imagens binárias
    
    // EROSÃO - 02_erosao + Binario1, Binario2, Binario3
    std::cout << "   → Aplicando Erosão..." << std::endl;
    
    // Adiciona 02_erosao.png na lista de erosão
    std::vector<std::string> imagensErosao = {"02_erosao.png", "Binario1.jpg", "Binario2.jpeg", "Binario3.jpg"};
    
    for (size_t i = 0; i < imagensErosao.size(); i++) {
        std::string nomeArquivo = imagensErosao[i];
        cv::Mat imagem = cv::imread("../data/model/" + nomeArquivo, cv::IMREAD_GRAYSCALE);
        
        if (imagem.empty()) {
            std::cerr << "     ✗ Erro ao carregar: " << nomeArquivo << std::endl;
            continue;
        }
        
        cv::Mat resultado = MorfologiaMatematica::erosao(imagem, elementoEstruturante);
        std::string prefixo = "../data/result/Morfologia Matemática/02_erosao_";
        
        if (nomeArquivo.find("02_erosao") != std::string::npos) prefixo += "02";
        else if (nomeArquivo.find("Binario1") != std::string::npos) prefixo += "Binario1";
        else if (nomeArquivo.find("Binario2") != std::string::npos) prefixo += "Binario2";
        else if (nomeArquivo.find("Binario3") != std::string::npos) prefixo += "Binario3";
        
        cv::imwrite(prefixo + "_original.png", imagem);
        cv::imwrite(prefixo + "_resultado.png", resultado);
        
        // Exibe as imagens
        mostrarImagem("Erosão - Original: " + nomeArquivo, imagem);
        mostrarImagem("Erosão - Resultado: " + nomeArquivo, resultado);
        
        std::cout << "     ✓ " << nomeArquivo << " processada" << std::endl;
    }
    std::cout << "     ⏸ Pressione qualquer tecla para continuar..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    // DILATAÇÃO - 03_dilatacao + Binario1, Binario2, Binario3
    std::cout << "   → Aplicando Dilatação..." << std::endl;
    
    std::vector<std::string> imagensDilatacao = {"03_dilatacao.png", "Binario1.jpg", "Binario2.jpeg", "Binario3.jpg"};
    
    for (size_t i = 0; i < imagensDilatacao.size(); i++) {
        std::string nomeArquivo = imagensDilatacao[i];
        cv::Mat imagem = cv::imread("../data/model/" + nomeArquivo, cv::IMREAD_GRAYSCALE);
        
        if (imagem.empty()) {
            std::cerr << "     ✗ Erro ao carregar: " + nomeArquivo << std::endl;
            continue;
        }
        
        cv::Mat resultado = MorfologiaMatematica::dilatacao(imagem, elementoEstruturante);
        std::string prefixo = "../data/result/Morfologia Matemática/03_dilatacao_";
        
        if (nomeArquivo.find("03_dilatacao") != std::string::npos) prefixo += "03";
        else if (nomeArquivo.find("Binario1") != std::string::npos) prefixo += "Binario1";
        else if (nomeArquivo.find("Binario2") != std::string::npos) prefixo += "Binario2";
        else if (nomeArquivo.find("Binario3") != std::string::npos) prefixo += "Binario3";
        
        cv::imwrite(prefixo + "_original.png", imagem);
        cv::imwrite(prefixo + "_resultado.png", resultado);
        
        // Exibe as imagens
        mostrarImagem("Dilatação - Original: " + nomeArquivo, imagem);
        mostrarImagem("Dilatação - Resultado: " + nomeArquivo, resultado);
        
        std::cout << "     ✓ " << nomeArquivo << " processada" << std::endl;
        }
    std::cout << "     ⏸ Pressione qualquer tecla para continuar..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    // ABERTURA - 04_abertura + Binario1, Binario2, Binario3
    std::cout << "   → Aplicando Abertura..." << std::endl;
    
    std::vector<std::string> imagensAbertura = {"04_abertura.png", "Binario1.jpg", "Binario2.jpeg", "Binario3.jpg"};
    
    for (size_t i = 0; i < imagensAbertura.size(); i++) {
        std::string nomeArquivo = imagensAbertura[i];
        cv::Mat imagem = cv::imread("../data/model/" + nomeArquivo, cv::IMREAD_GRAYSCALE);
        
        if (imagem.empty()) {
            std::cerr << "     ✗ Erro ao carregar: " + nomeArquivo << std::endl;
            continue;
        }
        
        cv::Mat resultado = MorfologiaMatematica::abertura(imagem, elementoEstruturante);
        std::string prefixo = "../data/result/Morfologia Matemática/04_abertura_";
        
        if (nomeArquivo.find("04_abertura") != std::string::npos) prefixo += "04";
        else if (nomeArquivo.find("Binario1") != std::string::npos) prefixo += "Binario1";
        else if (nomeArquivo.find("Binario2") != std::string::npos) prefixo += "Binario2";
        else if (nomeArquivo.find("Binario3") != std::string::npos) prefixo += "Binario3";
        
        cv::imwrite(prefixo + "_original.png", imagem);
        cv::imwrite(prefixo + "_resultado.png", resultado);
        
        // Exibe as imagens
        mostrarImagem("Abertura - Original: " + nomeArquivo, imagem);
        mostrarImagem("Abertura - Resultado: " + nomeArquivo, resultado);
        
        std::cout << "     ✓ " << nomeArquivo << " processada" << std::endl;
    }
    
    std::cout << "     ⏸ Pressione qualquer tecla para continuar..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    
    // FECHAMENTO - 05_fechamento + Binario1, Binario2, Binario3
    std::cout << "   → Aplicando Fechamento..." << std::endl;
    
    std::vector<std::string> imagensFechamento = {"05_fechamento.png", "Binario1.jpg", "Binario2.jpeg", "Binario3.jpg"};
    
    for (size_t i = 0; i < imagensFechamento.size(); i++) {
        std::string nomeArquivo = imagensFechamento[i];
        cv::Mat imagem = cv::imread("../data/model/" + nomeArquivo, cv::IMREAD_GRAYSCALE);
        
        if (imagem.empty()) {
            std::cerr << "     ✗ Erro ao carregar: " + nomeArquivo << std::endl;
            continue;
        }
        
        cv::Mat resultado = MorfologiaMatematica::fechamento(imagem, elementoEstruturante);
        std::string prefixo = "../data/result/Morfologia Matemática/05_fechamento_";
        
        if (nomeArquivo.find("05_fechamento") != std::string::npos) prefixo += "05";
        else if (nomeArquivo.find("Binario1") != std::string::npos) prefixo += "Binario1";
        else if (nomeArquivo.find("Binario2") != std::string::npos) prefixo += "Binario2";
        else if (nomeArquivo.find("Binario3") != std::string::npos) prefixo += "Binario3";
        
        cv::imwrite(prefixo + "_original.png", imagem);
        cv::imwrite(prefixo + "_resultado.png", resultado);
        
        // Exibe as imagens
        mostrarImagem("Fechamento - Original: " + nomeArquivo, imagem);
        mostrarImagem("Fechamento - Resultado: " + nomeArquivo, resultado);
        
        std::cout << "     ✓ " << nomeArquivo << " processada" << std::endl;
    }
    
    std::cout << "     ⏸ Pressione qualquer tecla para continuar..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    // LIMITES - 06_limites + Binario1, Binario2, Binario3
    std::cout << "   → Aplicando Limites (Interno e Externo)..." << std::endl;
    
    std::vector<std::string> imagensLimites = {"06_limites.png", "Binario1.jpg", "Binario2.jpeg", "Binario3.jpg"};
    
    for (size_t i = 0; i < imagensLimites.size(); i++) {
        std::string nomeArquivo = imagensLimites[i];
        cv::Mat imagem = cv::imread("../data/model/" + nomeArquivo, cv::IMREAD_GRAYSCALE);
        
        if (imagem.empty()) {
            std::cerr << "     ✗ Erro ao carregar: " + nomeArquivo << std::endl;
            continue;
        }
        
        cv::Mat limiteInterno = MorfologiaMatematica::limiteInterno(imagem, elementoEstruturante);
        cv::Mat limiteExterno = MorfologiaMatematica::limiteExterno(imagem, elementoEstruturante);
        
        std::string prefixo = "../data/result/Morfologia Matemática/06_limites_";
        
        if (nomeArquivo.find("06_limites") != std::string::npos) prefixo += "06";
        else if (nomeArquivo.find("Binario1") != std::string::npos) prefixo += "Binario1";
        else if (nomeArquivo.find("Binario2") != std::string::npos) prefixo += "Binario2";
        else if (nomeArquivo.find("Binario3") != std::string::npos) prefixo += "Binario3";
        
        cv::imwrite(prefixo + "_original.png", imagem);
        cv::imwrite(prefixo + "_interno.png", limiteInterno);
        cv::imwrite(prefixo + "_externo.png", limiteExterno);
        
        // Exibe as imagens
        mostrarImagem("Limites - Original: " + nomeArquivo, imagem);
        mostrarImagem("Limites - Interno: " + nomeArquivo, limiteInterno);
        mostrarImagem("Limites - Externo: " + nomeArquivo, limiteExterno);
        
        std::cout << "     ✓ " << nomeArquivo << " processada" << std::endl;
    }
    
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    int totalMorfologia = imagensBinarias.size() * 2 * 5 + imagensBinarias.size() * 3; // (original + resultado) * 5 operações + 3 imagens de limites extras

    // 3. IDENTIFICAÇÃO DE BORDAS
    // Lista de imagens em cinza: 07_bordas, Cinza4, Cinza7, Cinza8
    std::vector<std::string> imagensBordas = {
        "07_bordas.png", "Cinza4.jpeg", "Cinza7.jpeg", "Cinza8.jpeg"
    };
    
    int totalBordas = 0;
    for (const auto& nomeArquivo : imagensBordas) {
        cv::Mat imagemBordasOriginal = cv::imread("../data/model/" + nomeArquivo);
        
        if (imagemBordasOriginal.empty()) {
            std::cerr << "   ✗ Erro ao carregar: " << nomeArquivo << std::endl;
            continue;
        }
                
        // Converte para cinza
        cv::Mat imagemBordasCinza = ConversorTonsCinza::paraMediaPonderada(imagemBordasOriginal);
        
        // Roberts
        cv::Mat bordasRoberts = DetectorBordas::roberts(imagemBordasCinza);
        cv::Mat bordasRobertsLimiar = DetectorBordas::aplicarLimiar(bordasRoberts, 50);
        
        // Sobel
        cv::Mat bordasSobel = DetectorBordas::sobel(imagemBordasCinza);
        cv::Mat bordasSobelLimiar = DetectorBordas::aplicarLimiar(bordasSobel, 50);
        
        // Robinson
        cv::Mat bordasRobinson = DetectorBordas::robinson(imagemBordasCinza);
        cv::Mat bordasRobinsonLimiar = DetectorBordas::aplicarLimiar(bordasRobinson, 50);
        
        // Salva resultados na pasta organizada
        std::string prefixo = "../data/result/Identificação de Bordas/07_bordas_";
        if (nomeArquivo.find("07_bordas") != std::string::npos) {
            prefixo += "07_";
        } else if (nomeArquivo.find("Cinza4") != std::string::npos) {
            prefixo += "Cinza4_";
        } else if (nomeArquivo.find("Cinza7") != std::string::npos) {
            prefixo += "Cinza7_";
        } else if (nomeArquivo.find("Cinza8") != std::string::npos) {
            prefixo += "Cinza8_";
        }
        
        cv::imwrite(prefixo + "original.png", imagemBordasCinza);
        cv::imwrite(prefixo + "roberts.png", bordasRoberts);
        cv::imwrite(prefixo + "roberts_limiar.png", bordasRobertsLimiar);
        cv::imwrite(prefixo + "sobel.png", bordasSobel);
        cv::imwrite(prefixo + "sobel_limiar.png", bordasSobelLimiar);
        cv::imwrite(prefixo + "robinson.png", bordasRobinson);
        cv::imwrite(prefixo + "robinson_limiar.png", bordasRobinsonLimiar);
        
        // Exibe as imagens
        mostrarImagem("Bordas - Original: " + nomeArquivo, imagemBordasCinza);
        mostrarImagem("Roberts: " + nomeArquivo, bordasRoberts);
        mostrarImagem("Roberts (Limiarizado): " + nomeArquivo, bordasRobertsLimiar);
        mostrarImagem("Sobel: " + nomeArquivo, bordasSobel);
        mostrarImagem("Sobel (Limiarizado): " + nomeArquivo, bordasSobelLimiar);
        mostrarImagem("Robinson: " + nomeArquivo, bordasRobinson);
        mostrarImagem("Robinson (Limiarizado): " + nomeArquivo, bordasRobinsonLimiar);
        
        totalBordas += 7;
        std::cout << "     ✓ " << nomeArquivo << " processada (7 resultados)" << std::endl;
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    
    std::cout << "   ✓ Detecção de Bordas concluída - " << totalBordas << " imagens salvas" << std::endl;

    // FINALIZAÇÃO
    std::cout << "  ✓ PROCESSAMENTO M2.1 CONCLUÍDO COM SUCESSO!          " << std::endl;
    std::cout << "\nResumo do Processamento:" << std::endl;
    std::cout << "\n  1. Convolução Simples - 4 imagens" << std::endl;
    std::cout << "     • 01_convolucao.png, Cinza1.jpeg, Cinza2.jpeg, Cinza3.jpeg" << std::endl;
    std::cout << "     • Kernels: Passa-Baixa, Passa-Alta, Nitidez" << std::endl;
    std::cout << "     • Total: 16 imagens de resultado (4 originais + 12 processadas)" << std::endl;
    std::cout << "\n  2. Morfologia Matemática - 5 operações × 4 imagens cada" << std::endl;
    std::cout << "     • Erosão: 02_erosao.png + Binario1, Binario2, Binario3 (8 imagens)" << std::endl;
    std::cout << "     • Dilatação: 03_dilatacao.png + Binario1, Binario2, Binario3 (8 imagens)" << std::endl;
    std::cout << "     • Abertura: 04_abertura.png + Binario1, Binario2, Binario3 (8 imagens)" << std::endl;
    std::cout << "     • Fechamento: 05_fechamento.png + Binario1, Binario2, Binario3 (8 imagens)" << std::endl;
    std::cout << "     • Limites: 06_limites.png + Binario1, Binario2, Binario3 (12 imagens)" << std::endl;
    std::cout << "     • Total: 44 imagens de resultado" << std::endl;
    std::cout << "\n  3. Detecção de Bordas - 4 imagens" << std::endl;
    std::cout << "     • 07_bordas.png, Cinza4.jpeg, Cinza7.jpeg, Cinza8.jpeg" << std::endl;
    std::cout << "     • Operadores: Roberts, Sobel, Robinson (+ limiarizados)" << std::endl;
    std::cout << "     • Total: 28 imagens de resultado" << std::endl;
    std::cout << "\n📁 Todos os resultados salvos em: data/result/" << std::endl;
    std::cout << "   • Convolução Simples/" << std::endl;
    std::cout << "   • Morfologia Matemática/" << std::endl;
    std::cout << "   • Identificação de Bordas/" << std::endl;
    std::cout << "📊 Total de imagens processadas: ~88 imagens (16 + 44 + 28)" << std::endl;
    std::cout << "\n✅ Processamento concluído!" << std::endl;
    
    return 0;
}
