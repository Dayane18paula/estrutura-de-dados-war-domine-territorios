#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Estrutura que representa um território do jogo.
// Cada território possui um nome, uma cor que identifica o exército dono
// e a quantidade de tropas estacionadas nele.
typedef struct {
    char nome[30];   // nome do território
    char cor[10];    // cor do exército (dono)
    int tropas;      // quantidade de tropas
} Territorio;

// Função auxiliar: stringDuplicate
// Faz duplicação de string usando malloc para ser compatível com ISO C.
char* stringDuplicate(const char* origem) {
    if (origem == NULL) return NULL;
    size_t tamanho = strlen(origem) + 1;
    char* copia = (char*) malloc(tamanho);
    if (copia != NULL) {
        memcpy(copia, origem, tamanho);
    }
    return copia;
}

// Missões fixas (descrições), usadas para sorteio
#define TOTAL_MISSOES 5
#define MAX_MISSAO_LEN 128
const char* MISSOES_FIXAS_CONST[TOTAL_MISSOES] = {
    "Conquistar 2 territórios azuis",
    "Conquistar 3 territórios vermelhos",
    "Eliminar todas as tropas da cor Azul",
    "Manter pelo menos 1 território com cor Vermelha",
    "Conquistar território sem tropas"
};

// Função: atribuirMissao
// Sorteia uma missão de forma aleatória e copia a descrição para o buffer de destino.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes; // índice aleatório entre 0 e totalMissoes-1
    strcpy(destino, missoes[idx]);   // copia a missão para a string do jogador
}

// Função: exibirMissao
// Exibe a missão (somente no início, conforme requerimento)
void exibirMissao(const char* missao) {
    printf("Missão: %s\n", missao);
}

// Função: verificarMissao
// Verifica se a missão sorteada para o jogador foi cumprida.
// A lógica de verificação depende do texto exato da missão.
int verificarMissao(const char* missao, Territorio* mapa, int tamanho) {
    int i;
    int countAzul = 0, countVermelho = 0;

    // Conta quantos territórios pertencem a cada cor.
    for (i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, "Azul") == 0) {
            countAzul++;
        } else if (strcmp(mapa[i].cor, "Vermelho") == 0) {
            countVermelho++;
        }
    }

    // Verifica cada missão conhecida usando o estado atual do mapa.
    if (strcmp(missao, "Conquistar 2 territórios azuis") == 0) {
        return (countAzul >= 2);
    } else if (strcmp(missao, "Conquistar 3 territórios vermelhos") == 0) {
        return (countVermelho >= 3);
    } else if (strcmp(missao, "Eliminar todas as tropas da cor Azul") == 0) {
        // Retorna verdadeiro apenas se nenhum território Azul tiver tropas.
        for (i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0 && mapa[i].tropas > 0) return 0;
        }
        return 1;
    } else if (strcmp(missao, "Manter pelo menos 1 território com cor Vermelha") == 0) {
        // Retorna verdadeiro se existir ao menos um território Vermelho com tropas > 0.
        for (i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0) return 1;
        }
        return 0;
    } else if (strcmp(missao, "Conquistar território sem tropas") == 0) {
        // Missão cumprida se qualquer território estiver com zero tropas.
        for (i = 0; i < tamanho; i++) {
            if (mapa[i].tropas == 0) return 1;
        }
        return 0;
    }

    // Missão desconhecida: considera não cumprida.
    return 0;
}

// Função: validarAtaque
// Verifica se o ataque é permitido entre dois territórios.
// Regras implementadas:
// - não pode atacar o próprio território
// - o atacante deve ter mais de 1 tropa (deve deixar pelo menos uma tropa)
// - o defensor deve pertencer a um exército inimigo
bool validarAtaque(const Territorio* atacante, const Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) return false;
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: atacante e defensor possuem a mesma cor.\n");
        return false;
    }
    if (atacante->tropas <= 1) {
        printf("Ataque inválido: o território atacante precisa ter mais de 1 tropa para atacar.\n");
        return false;
    }
    return true;
}

// Função: atacar
// Simula o ataque entre atacante e defensor usando dados aleatórios (1-6)
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 0) {
        printf("Ataque inválido: atacante sem tropas.\n");
        return;
    }

    int rollA = (rand() % 6) + 1; // dado do atacante
    int rollD = (rand() % 6) + 1; // dado do defensor

    printf("\n[ATAQUE] %s (Tropas=%d) vs %s (Tropas=%d)\n",
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    printf("  Dados -> atacante: %d, defensor: %d\n", rollA, rollD);

    if (rollA > rollD) {
        // atacante vence
        int transfer = atacante->tropas / 2; // metade das tropas vão para o defensor
        if (transfer > 0) {
            defensor->tropas += transfer;
            atacante->tropas -= transfer;
        }
        // transfere cor (novo dono do território defensor)
        strcpy(defensor->cor, atacante->cor);
        printf("  Resultado: ATACANTE vence! Novo dono do território defensor: %s. Tropas transferidas: %d\n",
               defensor->cor, transfer);
    } else {
        // defensor vence ou empate
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("  Resultado: DEFENSOR vence (ou empate). Atacante perde 1 tropa.\n");
    }
}

// Função: exibirMapa
// Mostra todas as informações dos territórios na tela para o jogador.
void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Território %d: Nome=%s | Cor=%s | Tropas=%d\n",
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("====================\n");
}

// Função: cadastrarTerritoriosDinam
// Aloca dinamicamente o vetor de territórios e lê os dados fornecidos pelo usuário.
Territorio* cadastrarTerritoriosDinam(int n) {
    Territorio* mapa = (Territorio*) malloc(n * sizeof(Territorio));
    if (mapa == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("  Nome (max 29): ");
        scanf("%29s", mapa[i].nome);  // evita overflow do buffer

        printf("  Cor do exército: ");
        scanf("%9s", mapa[i].cor);   // evita overflow do buffer

        printf("  Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
    return mapa;
}

// Função: liberarMemoria
// Libera toda a memória alocada dinamicamente durante o jogo.
void liberarMemoria(Territorio* mapa,
                   char** missoesFixas, int totalMissoes,
                   char** missaoJogadores, int totalJogadores) {
    // libera as missões fixas
    if (missoesFixas != NULL) {
        for (int i = 0; i < totalMissoes; i++) {
            free(missoesFixas[i]);
        }
        // missoesFixas é um array local no stack, não deve ser liberado
    }

    // libera as missões dos jogadores
    if (missaoJogadores != NULL) {
        for (int i = 0; i < totalJogadores; i++) {
            free(missaoJogadores[i]);
        }
        free(missaoJogadores);
    }

    // libera o mapa
    free(mapa);
}

// ------------------- MAIN -------------------
// Função principal: controla todo o fluxo do jogo.
int main() {
    // Inicializa a semente do gerador para que os resultados de rand() variem a cada execução.
    srand((unsigned)time(NULL));

    int n;
    printf("WAR estruturado com missões estratégicas\n");
    printf("Informe o número total de territórios: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada inválida de número de territórios.\n");
        return 1;
    }

    // Cria o mapa de territórios dinamicamente com base no número informado.
    Territorio* mapa = cadastrarTerritoriosDinam(n);
    if (mapa == NULL) {
        printf("Erro na alocação de memória para o mapa.\n");
        return 1;
    }

    // Preparação das missões (dinâmicas)
    // Duplicamos as missões fixas para que cada string fique em memória alocada.
    char* missoesFixas[TOTAL_MISSOES];
    for (int i = 0; i < TOTAL_MISSOES; i++) {
        missoesFixas[i] = stringDuplicate(MISSOES_FIXAS_CONST[i]);
        // stringDuplicate aloca memória; precisamos liberar depois
        if (missoesFixas[i] == NULL) {
            printf("Erro na alocação de memória para as missões fixas.\n");
            for (int j = 0; j < i; j++) {
                free(missoesFixas[j]);
            }
            free(mapa);
            return 1;
        }
    }

    // Aloca as strings onde as missões dos jogadores serão armazenadas.
    const int totalJogadores = 2;
    char** missaoJogadores = (char**) malloc(totalJogadores * sizeof(char*));
    if (missaoJogadores == NULL) {
        printf("Erro na alocação de memória para missões dos jogadores.\n");
        liberarMemoria(mapa, missoesFixas, TOTAL_MISSOES, NULL, 0);
        return 1;
    }
    for (int i = 0; i < totalJogadores; i++) {
        missaoJogadores[i] = (char*) malloc(MAX_MISSAO_LEN);
        if (missaoJogadores[i] == NULL) {
            printf("Erro na alocação de memória para missão do jogador %d.\n", i + 1);
            liberarMemoria(mapa, missoesFixas, TOTAL_MISSOES, missaoJogadores, i);
            return 1;
        }
    }

    // Sorteia e atribui uma missão para cada jogador.
    atribuirMissao(missaoJogadores[0], missoesFixas, TOTAL_MISSOES);
    atribuirMissao(missaoJogadores[1], missoesFixas, TOTAL_MISSOES);

    // Exibição das missões apenas no início do jogo.
    printf("\nMissões sorteadas para cada jogador (início do jogo):\n");
    exibirMissao(missaoJogadores[0]);
    exibirMissao(missaoJogadores[1]);

    // Mostra o mapa inicial antes do primeiro ataque.
    exibirMapa(mapa, n);

    int vencedor = -1;
    int turno = 1;

    while (1) {
        printf("\n--- Turno %d ---\n", turno);
        // Leitura de ataque
        int idxAtacante, idxDefensor;
        printf("Selecione o atacante (1..%d): ", n);
        if (scanf("%d", &idxAtacante) != 1) break;
        printf("Selecione o defensor (1..%d): ", n);
        if (scanf("%d", &idxDefensor) != 1) break;

        if (idxAtacante < 1 || idxAtacante > n || idxDefensor < 1 || idxDefensor > n) {
            printf("Seleção de territórios fora do intervalo. Tente novamente.\n");
            continue;
        }
        if (idxAtacante == idxDefensor) {
            printf("Não é possível atacar o próprio território.\n");
            continue;
        }

        // Ajusta índices de usuário (1..n) para índices de vetor (0..n-1).
        Territorio* atacante = &mapa[idxAtacante - 1];
        Territorio* defensor = &mapa[idxDefensor - 1];

        // Valida a ação antes de executar o ataque.
        if (!validarAtaque(atacante, defensor)) {
            continue;
        }

        // Executa o ataque e atualiza os dados dos territórios.
        atacar(atacante, defensor);

        // Exibe o novo estado do mapa após o ataque.
        exibirMapa(mapa, n);

        // Verifica se algum dos jogadores cumpriu sua missão no estado atual.
        for (int j = 0; j < 2; j++) {
            if (verificarMissao(missaoJogadores[j], mapa, n)) {
                vencedor = j;
                break;
            }
        }
        if (vencedor != -1) break;

        // Pergunta se o usuário deseja continuar o jogo.
        char continuar = 'n';
        printf("Deseja continuar? (s/n): ");
        scanf(" %c", &continuar);
        if (continuar != 's' && continuar != 'S') break;

        turno++;
    }

    if (vencedor == 0) {
        printf("\nJogador 1 cumpriu a missão e vence! 🎉\n");
    } else if (vencedor == 1) {
        printf("\nJogador 2 cumpriu a missão e vence! 🎉\n");
    } else {
        printf("\nFim de jogo sem vencedor imediato.\n");
    }

    // Liberação de memória
    liberarMemoria(mapa, missoesFixas, TOTAL_MISSOES, missaoJogadores, totalJogadores);
    return 0;
}