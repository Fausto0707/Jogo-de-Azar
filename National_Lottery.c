#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

// Função para comparar números (usada pelo qsort)
int compararNumeros(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Função para gerar um jogo de loteria sem números repetidos
void gerarJogo(int jogo[], int numerosPorJogo, int valorMaximo) {
    int i, j, num;
    
    for (i = 0; i < numerosPorJogo; i++) {
        do {
            num = rand() % valorMaximo + 1;
            
            // Verifica se o número já existe no jogo
            for (j = 0; j < i; j++) {
                if (jogo[j] == num) break;
            }
        } while (j < i); // Garante que o número não se repita

        jogo[i] = num;
    }

    // Ordena os números do jogo para melhor visualização
    qsort(jogo, numerosPorJogo, sizeof(int), compararNumeros);
}

// Função para verificar se o usuário ganhou
int verificarVitoria(int jogoUsuario[], int jogoSorteado[], int numerosPorJogo) {
    int i, j, acertos = 0;

    for (i = 0; i < numerosPorJogo; i++) {
        for (j = 0; j < numerosPorJogo; j++) {
            if (jogoUsuario[i] == jogoSorteado[j]) {
                acertos++;
                break;
            }
        }
    }

    return acertos == numerosPorJogo;
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    int N, i, j;
    int numerosPorJogo = 6;
    int valorMaximo = 59;

    printf("Bem-vindo ao National Lottery (Reino Unido)!\n\n");

    printf("Digite o número de jogos que deseja gerar: ");
    if (scanf("%d", &N) != 1 || N < 1) {
        printf("Número de jogos inválido. Por favor, insira um número positivo.\n");
        return 1;
    }

    // Aloca dinamicamente a memória para armazenar os jogos
    int **jogos = malloc(N * sizeof(int *));
    if (jogos == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    for (i = 0; i < N; i++) {
        jogos[i] = malloc(numerosPorJogo * sizeof(int));
        if (jogos[i] == NULL) {
            printf("Erro ao alocar memória.\n");
            return 1;
        }
    }

    srand(time(NULL)); // Inicializa a semente de números aleatórios

    // Gera os jogos do usuário
    for (i = 0; i < N; i++) {
        gerarJogo(jogos[i], numerosPorJogo, valorMaximo);
    }

    // Gera o jogo sorteado
    int jogoSorteado[6];
    gerarJogo(jogoSorteado, numerosPorJogo, valorMaximo);

    // Exibe o jogo sorteado
    printf("\nJogo sorteado: ");
    for (i = 0; i < numerosPorJogo; i++) {
        printf("%d ", jogoSorteado[i]);
    }
    printf("\n");

    // Verifica se algum jogo do usuário ganhou
    int ganhou = 0;
    for (i = 0; i < N; i++) {
        if (verificarVitoria(jogos[i], jogoSorteado, numerosPorJogo)) {
            ganhou = 1;
            break;
        }
    }

    // Exibe o resultado
    if (ganhou) {
        printf("\nParabéns! Você ganhou!\n");
    } else {
        printf("\nInfelizmente, você perdeu. Tente novamente!\n");
    }

    // Exibe os jogos gerados pelo usuário
    printf("\nSeus jogos:\n");
    for (i = 0; i < N; i++) {
        printf("Jogo %d: ", i + 1);
        for (j = 0; j < numerosPorJogo; j++) {
            printf("%d ", jogos[i][j]);
        }
        printf("\n");
    }

    // Salva os jogos gerados em um arquivo
    FILE *arquivo = fopen("jogos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita. Certifique-se de que o programa tem permissões para criar arquivos no diretório.\n");
        return 1;
    }

    // Exibe e grava os jogos no arquivo
    fprintf(arquivo, "Jogos gerados:\n");
    for (i = 0; i < N; i++) {
        fprintf(arquivo, "Jogo %d: ", i + 1);
        for (j = 0; j < numerosPorJogo; j++) {
            fprintf(arquivo, "%d ", jogos[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    // Fecha o arquivo
    fclose(arquivo);
    printf("\nOs jogos foram salvos em 'jogos.txt'.\n");

    // Libera a memória alocada dinamicamente
    for (i = 0; i < N; i++) {
        free(jogos[i]);
    }
    free(jogos);

    return 0;
}
