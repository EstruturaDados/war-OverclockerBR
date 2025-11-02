// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
#include <stdio.h>
#include <string.h>

// Constante para o número fixo de territórios
#define NUM_TERRITORIOS 5

// 1. Criar a struct Territorio
typedef struct {
    char nome[50];
    char cor_exercito[20];
    int num_tropas;
} Territorio;

/**
 * @brief Limpa o caractere de nova linha (\n) que pode sobrar no buffer
 * após o uso de scanf para que o próximo fgets funcione corretamente.
 */
void limpar_buffer_simples() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Função para cadastrar os dados de todos os territórios.
 */
void cadastrar_territorios(Territorio mapa[]) {
    printf("🌎 --- Cadastro Inicial dos Territórios (%d) ---\n", NUM_TERRITORIOS);
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        
        // Entrada: Nome do Território (usando fgets)
        printf("Digite o Nome do Território: ");
        // Nível Novato: Não usamos fflush(stdin) que não é padrão
        if (fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin) != NULL) {
            // Remove o '\n' deixado pelo fgets
            mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        }

        // Entrada: Cor do Exército (usando fgets)
        printf("Digite a Cor do Exército: ");
        if (fgets(mapa[i].cor_exercito, sizeof(mapa[i].cor_exercito), stdin) != NULL) {
            // Remove o '\n' deixado pelo fgets
            mapa[i].cor_exercito[strcspn(mapa[i].cor_exercito, "\n")] = 0;
        }

        // Entrada: Número de Tropas (usando scanf)
        printf("Digite o Número de Tropas (apenas números): ");
        if (scanf("%d", &mapa[i].num_tropas) != 1) {
            // Em código de novato, muitas vezes tratamos o erro de forma simplista
            // ou assumimos que o usuário digitará certo.
            printf("⚠️ ERRO DE ENTRADA. TROPAS DEFINIDAS COMO 1.\n");
            mapa[i].num_tropas = 1;
        }
        
        // Limpar o buffer após scanf para garantir que o próximo fgets não pule
        limpar_buffer_simples();
    }
}

/**
 * @brief Função para exibir o estado atual do mapa.
 */
void exibir_mapa(const Territorio mapa[]) {
    printf("\n\n⚔️ *** Estado Atual do Mapa (%d Territórios) ***\n", NUM_TERRITORIOS);
    printf("----------------------------------------------\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Exército: %s\n", mapa[i].cor_exercito);
        printf("  Tropas: %d\n", mapa[i].num_tropas);
        printf("----------------------------------------------\n");
    }
}

// Implementação da função main focada apenas na Inicialização do Nível Novato
int main() {
    // 2. Usar um vetor estático de 5 elementos
    Territorio mapa[NUM_TERRITORIOS];
    
    printf("--- DESAFIO WAR ESTRUTURADO: NÍVEL NOVATO ---\n");

    // Limpeza de buffer inicial caso haja lixo (boa prática)
    limpar_buffer_simples(); 
    
    // 1. Configuração Inicial (Setup):
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    cadastrar_territorios(mapa);
    
    // 3. Exibir o estado atual do mapa
    exibir_mapa(mapa);
    
    // Simulação de Pausa Simples para Fechar
    printf("\n✅ Configuração Inicial Concluída. Pressione ENTER para sair.\n");
    getchar(); // Espera por um ENTER para fechar
    
    // Nível Novato não tem alocação dinâmica, então não há 'free' (limpeza)
    
    return 0;
}