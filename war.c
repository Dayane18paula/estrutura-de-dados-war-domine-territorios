#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20

// Estrutura que representa cada componente da torre de resgate.
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade;
} Componente;

long comparacoes_ordenacao = 0;
long comparacoes_busca = 0;

void selectionSortPrioridade(Componente componentes[], int quantidade);
void insertionSortTipo(Componente componentes[], int quantidade);
void bubbleSortNome(Componente componentes[], int quantidade);
int buscaBinariaPorNome(Componente componentes[], int quantidade, char chave[]);
void mostrarComponentes(const Componente componentes[], int quantidade);
void medirTempo(void (*algoritmo)(Componente[], int), Componente vetor[], int tamanho);
void cadastrarComponentes(Componente componentes[], int quantidade);
int lerInteiro(const char *prompt, int *valor, int min, int max);
void limparBufferEntrada(void);
void lerString(char destino[], int tamanho);

int main(void) {
    Componente componentes[MAX_COMPONENTES];
    int quantidade = 0;
    int ordemAtual = 0; // 1 = nome, 2 = tipo, 3 = prioridade
    int opcao;

    printf("=== Torre de Resgate: Preparacao de Componentes ===\n");
    while (!lerInteiro("Quantos componentes voce deseja cadastrar? (1 a 20): ", &quantidade, 1, MAX_COMPONENTES)) {
        ;
    }

    cadastrarComponentes(componentes, quantidade);

    do {
        printf("\n=== Menu de Estrategia ===\n");
        printf("1. Ordenar por nome (Bubble Sort)\n");
        printf("2. Ordenar por tipo (Insertion Sort)\n");
        printf("3. Ordenar por prioridade (Selection Sort)\n");
        printf("4. Buscar componente-chave por nome (Busca Binaria)\n");
        printf("5. Exibir componentes atuais\n");
        printf("0. Sair\n");
        if (!lerInteiro("Escolha uma opcao: ", &opcao, 0, 5)) {
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                medirTempo(bubbleSortNome, componentes, quantidade);
                ordemAtual = 1;
                mostrarComponentes(componentes, quantidade);
                break;
            case 2:
                medirTempo(insertionSortTipo, componentes, quantidade);
                ordemAtual = 2;
                mostrarComponentes(componentes, quantidade);
                break;
            case 3:
                medirTempo(selectionSortPrioridade, componentes, quantidade);
                ordemAtual = 3;
                mostrarComponentes(componentes, quantidade);
                break;
            case 4: {
                if (ordemAtual != 1) {
                    printf("\nA busca binaria so funciona quando os componentes estao ordenados por nome.\n");
                    break;
                }
                char chave[MAX_NOME];
                printf("Digite o nome do componente-chave para buscar: ");
                lerString(chave, MAX_NOME);
                int indice = buscaBinariaPorNome(componentes, quantidade, chave);
                printf("\nBusca binaria concluida com %ld comparacoes.\n", comparacoes_busca);
                if (indice >= 0) {
                    printf("Componente encontrado: %s | Tipo: %s | Prioridade: %d\n",
                           componentes[indice].nome,
                           componentes[indice].tipo,
                           componentes[indice].prioridade);
                    printf("O componente-chave destravou a ativacao da torre!\n");
                } else {
                    printf("Componente nao encontrado. Verifique o nome e tente novamente.\n");
                }
                break;
            }
            case 5:
                mostrarComponentes(componentes, quantidade);
                break;
            case 0:
                printf("\nPreparacao concluida. A torre de resgate esta pronta para a retirada.\n");
                break;
            default:
                printf("Opcao invalida. Escolha novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

// Registra componentes usando fgets para aceitar espacos e evita overflow.
void cadastrarComponentes(Componente componentes[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nComponente %d de %d\n", i + 1, quantidade);
        printf("Nome do componente: ");
        fflush(stdout);
        lerString(componentes[i].nome, MAX_NOME);
        printf("Tipo do componente: ");
        fflush(stdout);
        lerString(componentes[i].tipo, MAX_TIPO);

        while (!lerInteiro("Prioridade (1 a 10): ", &componentes[i].prioridade, 1, 10)) {
            ;
        }
    }
}

// Mostra a lista de componentes com seu nome, tipo e prioridade.
void mostrarComponentes(const Componente componentes[], int quantidade) {
    printf("\n=== Componentes da Torre de Resgate ===\n");
    printf("%-3s | %-28s | %-18s | %-10s\n", "#", "Nome", "Tipo", "Prioridade");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%-3d | %-28s | %-18s | %-10d\n",
               i + 1,
               componentes[i].nome,
               componentes[i].tipo,
               componentes[i].prioridade);
    }
}

// Gera uma busca binaria otimizada para encontrar o componente por nome.
int buscaBinariaPorNome(Componente componentes[], int quantidade, char chave[]) {
    int baixo = 0;
    int alto = quantidade - 1;
    comparacoes_busca = 0;

    while (baixo <= alto) {
        int meio = (baixo + alto) / 2;
        comparacoes_busca++;
        int resultado = strcmp(chave, componentes[meio].nome);

        if (resultado == 0) {
            return meio;
        }
        if (resultado < 0) {
            alto = meio - 1;
        } else {
            baixo = meio + 1;
        }
    }

    return -1;
}

// Avalia o tempo de execucao e compara o algoritmo de ordenacao escolhido.
void medirTempo(void (*algoritmo)(Componente[], int), Componente vetor[], int tamanho) {
    clock_t inicio = clock();
    algoritmo(vetor, tamanho);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nOrdenacao concluida em %.6f segundos com %ld comparacoes.\n",
           tempo,
           comparacoes_ordenacao);
}

// Ordena por nome usando bubble sort e conta comparacoes de strings.
void bubbleSortNome(Componente componentes[], int quantidade) {
    comparacoes_ordenacao = 0;
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            comparacoes_ordenacao++;
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                Componente temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
}

// Ordena por tipo usando insertion sort e conta comparacoes de strings.
void insertionSortTipo(Componente componentes[], int quantidade) {
    comparacoes_ordenacao = 0;
    for (int i = 1; i < quantidade; i++) {
        Componente chave = componentes[i];
        int j = i - 1;

        while (j >= 0) {
            comparacoes_ordenacao++;
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) {
                componentes[j + 1] = componentes[j];
                j--;
            } else {
                break;
            }
        }
        componentes[j + 1] = chave;
    }
}

// Ordena por prioridade usando selection sort e conta comparacoes numéricas.
void selectionSortPrioridade(Componente componentes[], int quantidade) {
    comparacoes_ordenacao = 0;
    for (int i = 0; i < quantidade - 1; i++) {
        int indiceMenor = i;
        for (int j = i + 1; j < quantidade; j++) {
            comparacoes_ordenacao++;
            if (componentes[j].prioridade < componentes[indiceMenor].prioridade) {
                indiceMenor = j;
            }
        }
        if (indiceMenor != i) {
            Componente temp = componentes[i];
            componentes[i] = componentes[indiceMenor];
            componentes[indiceMenor] = temp;
        }
    }
}

// Le uma string com fgets e remove o caracter de nova linha.
void lerString(char destino[], int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    }
}

// Le um inteiro usando fgets e sscanf para evitar problemas com buffers.
int lerInteiro(const char *prompt, int *valor, int min, int max) {
    char buffer[64];
    int numero;
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%d", &numero) != 1) {
        printf("Entrada invalida. Digite um numero valido.\n");
        return 0;
    }
    if (numero < min || numero > max) {
        printf("Valor fora do intervalo. Digite entre %d e %d.\n", min, max);
        return 0;
    }
    *valor = numero;
    return 1;
}

// Limpa o resto do buffer de entrada para evitar leituras indesejadas.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}
