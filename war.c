// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

    return 0;
}

Territorio* alocarTerritorios(int quantidade) {
    Territorio* mapa = calloc(quantidade, sizeof(Territorio));
    return mapa;
}

void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: ");
        scanf("%29s", mapa[i].nome);
        printf("Cor: ");
        scanf("%9s", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
        if (mapa[i].tropas < 1) {
            mapa[i].tropas = 1;
        }
    }
}

void exibirTerritorios(const Territorio* mapa, int quantidade) {
    printf("\n=== Estado Atual dos Territorios ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d) %s - Cor: %s - Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

int escolherIndice(int quantidade, const char* papel) {
    int escolha;
    printf("Escolha o territorio %s (1 a %d): ", papel, quantidade);
    while (scanf("%d", &escolha) != 1 || escolha < 1 || escolha > quantidade) {
        limparBufferEntrada();
        printf("Entrada invalida. Digite um numero entre 1 e %d: ", quantidade);
    }
    limparBufferEntrada();
    return escolha - 1;
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\nRolagem de dados: Atacante %s -> %d | Defensor %s -> %d\n",
           atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas < 1) {
            tropasTransferidas = 1;
        }
        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas;
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';
        printf("O ataque foi bem-sucedido! %s conquistou %s e transferiu %d tropas.\n",
               atacante->nome, defensor->nome, tropasTransferidas);
    } else {
        atacante->tropas -= 1;
        if (atacante->tropas < 0) {
            atacante->tropas = 0;
        }
        printf("O ataque falhou. %s perdeu 1 tropa.\n", atacante->nome);
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}
