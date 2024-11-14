#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINHAS 20
#define MAX_COLUNAS 20

// Inicializa o tabuleiro com um valor (ex: '0' ou '-')
void inicializarTabuleiro(char tabuleiro[MAX_LINHAS][MAX_COLUNAS], int linhas, int colunas, char valor) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = valor;
        }
    }
}

// Mostra o tabuleiro visível para o jogador
void exibirTabuleiro(char tabuleiro[MAX_LINHAS][MAX_COLUNAS], int linhas, int colunas) {
    printf("   ");
    for (int i = 0; i < colunas; i++) {
        printf("%2d ", i); // Mostra os números das colunas
    }
    printf("\n");
   
    for (int i = 0; i < linhas; i++) {
        printf("%2d ", i); // Mostra os números das linhas
        for (int j = 0; j < colunas; j++) {
            printf("%2c ", tabuleiro[i][j]); // Mostra o conteúdo das células
        }
        printf("\n");
    }
}

// Calcula quantas minas colocar, baseado na dificuldade
int calcularNumeroDeMinas(int linhas, int colunas, int dificuldade) {
    float porcentagem;
    if (dificuldade == 1) porcentagem = 0.4;  // 40% de minas
    else if (dificuldade == 2) porcentagem = 0.25; // 25% de minas
    else porcentagem = 0.1;  // 10% de minas
   
    return (int)(linhas * colunas * porcentagem); // Calcula o número total de minas
}

// Coloca minas aleatórias no tabuleiro
void colocarMinas(char tabuleiro[MAX_LINHAS][MAX_COLUNAS], int linhas, int colunas, int numMinas) {
    int minasColocadas = 0;
    srand(time(NULL)); // Inicia o gerador de números aleatórios
   
    while (minasColocadas < numMinas) {
        int linha = rand() % linhas;
        int coluna = rand() % colunas;
       
        if (tabuleiro[linha][coluna] != '*') { // Se não houver mina lá
            tabuleiro[linha][coluna] = '*'; // Coloca uma mina
            minasColocadas++; // Aumenta a quantidade de minas colocadas
        }
    }
}

// Conta as minas ao redor de uma célula
int contarMinasAoRedor(char tabuleiro[MAX_LINHAS][MAX_COLUNAS], int linha, int coluna, int linhas, int colunas) {
    int contagem = 0;
   
    // Verifica as células ao redor da célula escolhida
    for (int i = linha - 1; i <= linha + 1; i++) {
        for (int j = coluna - 1; j <= coluna + 1; j++) {
            if (i >= 0 && i < linhas && j >= 0 && j < colunas && tabuleiro[i][j] == '*') {
                contagem++; // Conta se houver uma mina
            }
        }
    }
   
    return contagem; // Retorna a quantidade de minas ao redor
}

// Revela a célula escolhida e as adjacentes se não tiver minas ao redor
void revelarCampo(char tabuleiroMinas[MAX_LINHAS][MAX_COLUNAS], char tabuleiroVisivel[MAX_LINHAS][MAX_COLUNAS], int linha, int coluna, int linhas, int colunas) {
    if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas || tabuleiroVisivel[linha][coluna] != '-') return;
   
    int minasAoRedor = contarMinasAoRedor(tabuleiroMinas, linha, coluna, linhas, colunas);
    tabuleiroVisivel[linha][coluna] = minasAoRedor + '0'; // Mostra o número de minas ao redor

    // Se não houver minas ao redor, revela as células vizinhas
    if (minasAoRedor == 0) {
        for (int i = linha - 1; i <= linha + 1; i++) {
            for (int j = coluna - 1; j <= coluna + 1; j++) {
                if (i >= 0 && i < linhas && j >= 0 && j < colunas && tabuleiroVisivel[i][j] == '-') {
                    revelarCampo(tabuleiroMinas, tabuleiroVisivel, i, j, linhas, colunas);
                }
            }
        }
    }
}

// Pede para o jogador escolher o tamanho do campo
void obterTamanhoCampo(int *linhas, int *colunas) {
    int opcao;
    printf("Escolha o tamanho do campo:\n");
    printf("1 - Grande (20x20)\n");
    printf("2 - Médio (15x15)\n");
    printf("3 - Pequeno (10x10)\n");
    printf("Opção: ");
    scanf("%d", &opcao);
   
    // Define o tamanho baseado na opção
    if (opcao == 1) { *linhas = 20; *colunas = 20; }
    else if (opcao == 2) { *linhas = 15; *colunas = 15; }
    else { *linhas = 10; *colunas = 10; }
}

// Pede para o jogador escolher o nível de dificuldade
int obterDificuldade() {
    int dificuldade;
    printf("Escolha a dificuldade:\n");
    printf("1 - Difícil (40%% de bombas)\n");
    printf("2 - Fácil (25%% de bombas)\n");
    printf("3 - Muito fácil (10%% de bombas)\n");
    printf("Opção: ");
    scanf("%d", &dificuldade);
    return dificuldade; // Retorna a dificuldade escolhida
}

// Função principal do jogo
int main() {
    char tabuleiroMinas[MAX_LINHAS][MAX_COLUNAS];
    char tabuleiroVisivel[MAX_LINHAS][MAX_COLUNAS];
    int linhas, colunas, dificuldade, numMinas;
    int linha, coluna;
    int movimentosRestantes;
    int perdeu = 0;
   
    // Pede o tamanho do campo e a dificuldade
    obterTamanhoCampo(&linhas, &colunas);
    dificuldade = obterDificuldade();
   
    // Calcula o número de minas
    numMinas = calcularNumeroDeMinas(linhas, colunas, dificuldade);
    movimentosRestantes = (linhas * colunas) - numMinas;
   
    // Inicializa os tabuleiros
    inicializarTabuleiro(tabuleiroMinas, linhas, colunas, '0');
    inicializarTabuleiro(tabuleiroVisivel, linhas, colunas, '-');
   
    // Coloca as minas aleatoriamente
    colocarMinas(tabuleiroMinas, linhas, colunas, numMinas);
   
    printf("Bem-vindo ao Campo Minado!\n");
   
    // Inicia o loop do jogo
    while (movimentosRestantes > 0 && !perdeu) {
        exibirTabuleiro(tabuleiroVisivel, linhas, colunas); // Mostra o tabuleiro visível
        printf("Movimentos restantes: %d\n", movimentosRestantes);
        printf("Digite a linha e a coluna (ex: 0 1): ");
        scanf("%d %d", &linha, &coluna);
       
        // Verifica se a célula escolhida está dentro do campo
        if (linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas) {
            printf("Coordenada inválida! Tente novamente.\n");
            continue;
        }
       
        // Revela a célula ou verifica se o jogador perdeu
        if (tabuleiroMinas[linha][coluna] == '*') {
            perdeu = 1; // Jogador perdeu se clicou numa mina
        } else {
            revelarCampo(tabuleiroMinas, tabuleiroVisivel, linha, coluna, linhas, colunas);
            movimentosRestantes--; // Diminui os movimentos restantes
        }
    }
   
    // Exibe o resultado do jogo
    if (perdeu) {
        printf("BOOM! Você pisou em uma mina. Game Over!\n");
    } else {
        printf("Parabéns! Você venceu!\n");
    }
   
    // Mostra o tabuleiro completo no final
    printf("Tabuleiro completo:\n");
    exibirTabuleiro(tabuleiroMinas, linhas, colunas);
   
    return 0;
}
