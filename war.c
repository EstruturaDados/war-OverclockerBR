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

// --- ENUMS e STRUCTS ---

typedef enum {
    MISSAO_DESTRUIR_VERDE,
    MISSAO_CONQUISTAR_3
} TipoMissao;

typedef struct {
    char nome[50];
    char cor_exercito[20];
    int num_tropas;
} Territorio;

typedef struct {
    char cor[20];
    TipoMissao missao;
    int territorios_iniciais;
} Jogador;


// ----------------------------------------------------------------------
// --- I. FUNÇÕES AUXILIARES DE I/O E VALIDAÇÃO ---
// ----------------------------------------------------------------------

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ler_inteiro_valido(const char *const prompt, const int min_val) {
    char buffer[MAX_INPUT_BUFFER];
    long temp_num;
    char *endptr;
    
    do {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue; 
        }

        temp_num = strtol(buffer, &endptr, 10);
        
        if (endptr != buffer && (*endptr == '\n' || *endptr == '\0') && temp_num >= min_val && temp_num <= 2147483647) {
            return (int)temp_num; 
        } else {
            if (min_val == 1) {
                printf("⚠️ Erro: Entrada inválida. Digite apenas números inteiros (Mínimo 1).\n");
            } else {
                printf("⚠️ Erro: Entrada inválida.\n");
            }
        }
        
    } while (1);
}

// ----------------------------------------------------------------------
// --- II. FUNÇÕES DE INICIALIZAÇÃO E MAPA ---
// ----------------------------------------------------------------------

void inicializar_territorio(Territorio *const t, const char *nome, const char *cor, const int tropas) {
    strcpy(t->nome, nome);
    strcpy(t->cor_exercito, cor);
    t->num_tropas = tropas;
}

void inicializar_mapa(Territorio *const mapa) {
    inicializar_territorio(&mapa[0], "Brasil", "Azul", 5);
    inicializar_territorio(&mapa[1], "Argentina", "Azul", 3);
    inicializar_territorio(&mapa[2], "Peru", "Vermelho", 4);
    inicializar_territorio(&mapa[3], "Chile", "Verde", 5);
    inicializar_territorio(&mapa[4], "Colombia", "Vermelho", 2);
}

void atribuir_missao(Jogador *const jogador) {
    jogador->cor[0] = '\0';
    
    int sorteio = rand() % 2; 
    
    if (sorteio == 0) {
        jogador->missao = MISSAO_DESTRUIR_VERDE;
        printf("\n🎯 Missão Designada: Destruir o exército Verde.\n");
    } else {
        jogador->missao = MISSAO_CONQUISTAR_3;
        printf("\n🎯 Missão Designada: Conquistar 3 territórios (total 5 ou mais).\n");
    }
}

/**
 * @brief Exibe o estado atual do mapa em formato tabular (in-line).
 */
void exibir_mapa(const Territorio *const mapa) {
    printf("\n\n⚔️ *** ESTADO ATUAL DO MAPA ***\n");
    printf("-------------------------------------------------------------------\n");
    printf("| ID | Território  | Exército | Tropas |\n");
    printf("-------------------------------------------------------------------\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        // Imprime cada território em uma linha compacta
        printf("| %-2d | %-11s | %-8s | %-6d |\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].cor_exercito, 
               mapa[i].num_tropas);
    }
    printf("-------------------------------------------------------------------\n");
}


// ----------------------------------------------------------------------
// --- III. FUNÇÕES DE COMBATE (Não Alteradas) ---
// ----------------------------------------------------------------------

int rolar_dado() {
    return (rand() % 6) + 1;
}

void fase_ataque(Territorio *const mapa) {
    int id_atacante, id_defensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    
    do {
        id_atacante = ler_inteiro_valido("Escolha o Território ATACANTE (1 a 5): ", 1);
        if (id_atacante < 1 || id_atacante > NUM_TERRITORIOS || mapa[id_atacante - 1].num_tropas <= 1) {
            printf("⚠️ Escolha inválida. O atacante deve existir e ter mais de 1 tropa.\n");
        } else {
            break;
        }
    } while (1);

    do {
        id_defensor = ler_inteiro_valido("Escolha o Território DEFENSOR (1 a 5): ", 1);
        if (id_defensor < 1 || id_defensor > NUM_TERRITORIOS || id_atacante == id_defensor) {
            printf("⚠️ Escolha inválida. O defensor deve existir e não ser o atacante.\n");
        } else {
            break;
        }
    } while (1);

    Territorio *atacante = &mapa[id_atacante - 1];
    Territorio *defensor = &mapa[id_defensor - 1];
    
    int dado_ataque = rolar_dado();
    int dado_defesa = rolar_dado();
    
    printf("\n🔥 BATALHA: %s vs %s 🔥\n", atacante->nome, defensor->nome);
    printf("Dados: Atacante (%d) | Defensor (%d)\n", dado_ataque, dado_defesa);
    
    if (dado_ataque > dado_defesa) {
        defensor->num_tropas--;
        printf("✅ ATAQUE BEM-SUCEDIDO! Defensor perdeu 1 tropa.\n");
    } else { 
        atacante->num_tropas--;
        printf("❌ DEFESA BEM-SUCEDIDA! Atacante perdeu 1 tropa.\n");
    }
    
    if (defensor->num_tropas <= 0) {
        printf("\n👑 CONQUISTA! %s tomou %s!\n", atacante->cor_exercito, defensor->nome);
        
        strcpy(defensor->cor_exercito, atacante->cor_exercito);
        defensor->num_tropas = 1;
        if (atacante->num_tropas > 0) {
            atacante->num_tropas--;
        }
    }
}

// ----------------------------------------------------------------------
// --- IV. FUNÇÕES DE MISSÃO E VITÓRIA (Não Alteradas) ---
// ----------------------------------------------------------------------

int verificar_missao(const Territorio *const mapa, const Jogador *const jogador) {
    int territorios_conquistados = 0;
    int tropas_verde = 0;
    
    if (jogador->missao == MISSAO_DESTRUIR_VERDE) {
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor_exercito, "Verde") == 0) {
                tropas_verde += mapa[i].num_tropas;
            }
        }
        
        printf("\n-- VERIFICAÇÃO DE MISSÃO --\n");
        printf("Objetivo: Destruir o exército Verde.\n");
        printf("Status: %d tropas Verdes restantes.\n", tropas_verde);
        
        return (tropas_verde == 0) ? 1 : 0;
        
    } else if (jogador->missao == MISSAO_CONQUISTAR_3) {
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor_exercito, "Azul") == 0) {
                territorios_conquistados++;
            }
        }
        
        printf("\n-- VERIFICAÇÃO DE MISSÃO --\n");
        printf("Objetivo: Conquistar 3 territórios (Total: %d).\n", jogador->territorios_iniciais + 3);
        printf("Status: %d territórios controlados por Azul.\n", territorios_conquistados);
        
        return (territorios_conquistados >= (jogador->territorios_iniciais + 3)) ? 1 : 0;
    }
    
    return 0;
}

// ----------------------------------------------------------------------
// --- V. FUNÇÃO PRINCIPAL (Não Alterada) ---
// ----------------------------------------------------------------------

int main() {
    
    srand((unsigned int)time(NULL)); 
    
    Territorio *mapa = NULL;
    Jogador jogador;
    int escolha_jogador = -1; 
    
    printf("--- DESAFIO WAR ESTRUTURADO: NÍVEL MESTRE ---\n");
    
    mapa = (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    if (mapa == NULL) {
        fprintf(stderr, "ERRO FATAL: Falha na alocação de memória.\n");
        return 1;
    }
    
    inicializar_mapa(mapa);
    
    strcpy(jogador.cor, "Azul");
    jogador.territorios_iniciais = 2;
    
    atribuir_missao(&jogador);
    
    printf("\n--- O JOGO COMEÇA ---\n");
    
    do {
        exibir_mapa(mapa); // <-- Nova visualização in-line aqui
        
        printf("\n\n--- MENU DE AÇÕES (%s) ---\n", jogador.cor);
        printf("1. Atacar\n");
        printf("2. Verificar Missão\n");
        printf("0. Sair do Jogo\n");
        
        escolha_jogador = ler_inteiro_valido("Escolha: ", 0); 
        
        switch (escolha_jogador) {
            case 1:
                fase_ataque(mapa);
                break;
            case 2:
                if (verificar_missao(mapa, &jogador)) {
                    printf("\n🏆 MISSAO CUMPRIDA! VOCÊ VENCEU O JOGO! 🏆\n");
                    escolha_jogador = 0;
                }
                break;
            case 0:
                printf("\nEncerrando o Desafio WAR Estruturado.\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

        if (escolha_jogador != 0) {
            printf("\nPressione ENTER para continuar...");
            limpar_buffer();
        }

    } while (escolha_jogador != 0);

    if (mapa != NULL) {
        free(mapa);
        printf("\n🧹 Memória do mapa liberada (free()).\n\n");
    }
    
    return 0;
}