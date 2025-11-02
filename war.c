// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// --- Configurações ---
#define NUM_TERRITORIOS 5
#define MAX_INPUT_BUFFER 100

// --- 🧩 Nível Novato: Struct ---
typedef struct {
    char nome[50];
    char cor_exercito[20];
    int num_tropas;
} Territorio;

// --- Funções Auxiliares de I/O ---

/**
 * @brief Limpa o buffer de entrada (stdin).
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Função robusta para ler um número inteiro dentro de um intervalo.
 * @param prompt A mensagem a ser exibida ao usuário.
 * @param min_val Valor mínimo aceitável (inclusive).
 * @return O número inteiro válido lido.
 */
int ler_inteiro_valido(const char *prompt, int min_val) {
    char buffer[MAX_INPUT_BUFFER];
    long temp_num;
    char *endptr;
    
    do {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("⚠️ Erro de leitura. Tente novamente.\n");
            continue; 
        }

        temp_num = strtol(buffer, &endptr, 10);
        
        // --- Condição de Validação (Ajustada para aceitar min_val) ---
        if (endptr != buffer && (*endptr == '\n' || *endptr == '\0') && temp_num >= min_val && temp_num <= 2147483647) {
            return (int)temp_num; 
        } else {
            // Se a validação for para tropas, a mensagem é específica.
            if (min_val == 1) {
                printf("⚠️ Erro: Entrada inválida. Digite apenas números inteiros (Mínimo 1).\n");
            } else { // Se a validação for para o menu (min_val = 0 ou 1)
                printf("⚠️ Erro: Entrada inválida.\n");
            }
        }
        
    } while (1);
}

// --- 🧩 Nível Novato: Cadastro ---

/**
 * @brief Cadastra os dados iniciais dos territórios.
 */
void cadastrar_territorios(Territorio *mapa) {
    printf("🌎 --- Cadastro Inicial dos Territórios (%d) ---\n", NUM_TERRITORIOS);
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        
        // Leitura do Nome (fgets)
        printf("Digite o Nome do Território: ");
        if (fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin) != NULL) {
            mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        }

        // Leitura da Cor (fgets)
        printf("Digite a Cor do Exército: ");
        if (fgets(mapa[i].cor_exercito, sizeof(mapa[i].cor_exercito), stdin) != NULL) {
            mapa[i].cor_exercito[strcspn(mapa[i].cor_exercito, "\n")] = 0;
        }

        // Leitura das Tropas (mínimo 1)
        mapa[i].num_tropas = ler_inteiro_valido("Digite o Número de Tropas (Mínimo 1): ", 1);
    }
}

/**
 * @brief Exibe o estado atual do mapa.
 */
void exibir_mapa(const Territorio *mapa) {
    printf("\n\n⚔️ *** ESTADO ATUAL DO MAPA (%d Territórios) ***\n", NUM_TERRITORIOS);
    printf("----------------------------------------------\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Exército: %s\n", mapa[i].cor_exercito);
        printf("  Tropas: %d\n", mapa[i].num_tropas);
        printf("----------------------------------------------\n");
    }
}


// --- 🧗‍♂️ Nível Aventureiro: Batalhas e Lógica de Jogo ---

/**
 * @brief Simula um dado de batalha (1 a 6).
 * @return O valor do dado sorteado.
 */
int rolar_dado() {
    return (rand() % 6) + 1; // Gera um número entre 1 e 6
}

/**
 * @brief Simula a fase de ataque entre dois territórios.
 */
void fase_ataque(Territorio *mapa) {
    int id_atacante, id_defensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    
    // Escolha do Atacante (min_val = 1)
    do {
        id_atacante = ler_inteiro_valido("Escolha o Território ATACANTE (1 a 5): ", 1);
        if (id_atacante < 1 || id_atacante > NUM_TERRITORIOS) {
            printf("⚠️ ID de território inválido.\n");
        } else if (mapa[id_atacante - 1].num_tropas <= 1) {
            printf("⚠️ O território atacante deve ter mais de 1 tropa para atacar.\n");
        } else {
            break;
        }
    } while (1);

    // Escolha do Defensor (min_val = 1)
    do {
        id_defensor = ler_inteiro_valido("Escolha o Território DEFENSOR (1 a 5): ", 1);
        if (id_defensor < 1 || id_defensor > NUM_TERRITORIOS) {
            printf("⚠️ ID de território inválido.\n");
        } else if (id_atacante == id_defensor) {
            printf("⚠️ Não é possível atacar o próprio território.\n");
        } else {
            break;
        }
    } while (1);

    Territorio *atacante = &mapa[id_atacante - 1];
    Territorio *defensor = &mapa[id_defensor - 1];
    
    // ----------------------------------------------------
    // Simulação da Batalha
    
    int dado_ataque = rolar_dado();
    int dado_defesa = rolar_dado();
    
    printf("\n🔥 BATALHA: %s (%s) vs %s (%s) 🔥\n", 
           atacante->nome, atacante->cor_exercito, 
           defensor->nome, defensor->cor_exercito);
    
    printf("Dados Sorteados:\n");
    printf("  Atacante: %d\n", dado_ataque);
    printf("  Defensor: %d\n", dado_defesa);
    
    // Lógica da Batalha:
    if (dado_ataque > dado_defesa) {
        defensor->num_tropas--;
        printf("\n✅ ATAQUE BEM-SUCEDIDO! Defensor perdeu 1 tropa.\n");
    } else { // Inclui empate e derrota
        atacante->num_tropas--;
        printf("\n❌ DEFESA BEM-SUCEDIDA! Atacante perdeu 1 tropa.\n");
    }
    
    // Lógica de Conquista
    if (defensor->num_tropas <= 0) {
        printf("\n👑 CONQUISTA! %s conquistou %s!\n", atacante->nome, defensor->nome);
        
        // Transferência de controle e uma tropa
        strcpy(defensor->cor_exercito, atacante->cor_exercito);
        defensor->num_tropas = 1;
        
        // Verifica se o atacante ainda tem tropas
        if (atacante->num_tropas > 0) {
            atacante->num_tropas--;
        }
    }
}

// --- Função Principal: Game Loop ---

int main() {
    
    srand((unsigned int)time(NULL)); 
    
    Territorio *mapa = NULL;
    int escolha_jogador = -1; 
    
    // 1. Configuração Inicial (Setup): Alocação Dinâmica
    printf("Configurando ambiente...\n");
    
    mapa = (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    if (mapa == NULL) {
        fprintf(stderr, "ERRO FATAL: Falha na alocação de memória.\n");
        return 1;
    }
    
    // Cadastro dos Territórios (chamado sem limpeza prévia do buffer, resolvendo o 1º ENTER)
    cadastrar_territorios(mapa);
    
    printf("\n--- NÍVEL AVENTUREIRO INICIADO ---\n");
    
    // 2. Laço Principal do Jogo (Game Loop):
    do {
        exibir_mapa(mapa);
        
        printf("\n\n--- MENU DE AÇÕES ---\n");
        printf("1. Iniciar Batalha (Atacar)\n");
        printf("0. Sair do Jogo\n");
        
        // Leitura da escolha do menu (agora min_val é 0 para aceitar a saída)
        escolha_jogador = ler_inteiro_valido("Escolha: ", 0); 
        
        switch (escolha_jogador) {
            case 1:
                fase_ataque(mapa);
                
                // Pede ENTER para o usuário pausar após uma ação, mas não interfere no fluxo de entrada.
                printf("\n\n-- Rodada Concluída --\n");
                printf("Pressione ENTER para continuar...");
                limpar_buffer(); // Limpa a linha pendente do número e espera por um novo ENTER
                break;
            case 0:
                printf("\nEncerrando o Desafio WAR Estruturado.\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                // Pede ENTER para continuar após uma opção inválida
                printf("Pressione ENTER para continuar...");
                limpar_buffer();
                break;
        }

    } while (escolha_jogador != 0);

    // 3. Limpeza: Liberação de memória
    if (mapa != NULL) {
        free(mapa);
        printf("\n🧹 Memória do mapa liberada (free()).\n\n");
    }
    
    return 0;
}