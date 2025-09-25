#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 50

typedef struct {
    char nome[30];
    char tipo[30];
    int prioridade; // 1 a 5
    int quantidade; // quantidade do componente
} Item;

void listarComponentes(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("\nNenhum componente na mochila.\n");
        return;
    }

    printf("\n-----------------------------------------------------------------------------\n");
    printf("| %-3s | %-20s | %-15s | %-10s | %-10s |\n",
           "Nº", "Nome", "Tipo", "Prioridade", "Quantidade");
    printf("-----------------------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        printf("| %-3d | %-20s | %-15s | %-10d | %-10d |\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].prioridade,
               mochila[i].quantidade);
    }

    printf("-----------------------------------------------------------------------------\n\n");
}

// BUBBLE SORT (NOME)
void bubbleSort(Item mochila[], int qtd, long *comparacoes) {
    *comparacoes = 0;
    for (int pass = 0; pass < qtd - 1; pass++) {
        for (int j = 0; j < qtd - pass - 1; j++) {
            (*comparacoes)++;
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                Item tmp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = tmp;
            }
        }
    }
}

// INSERTION SORT (TIPO)
void insertionSort(Item mochila[], int qtd, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < qtd; i++) {
        Item key = mochila[i];
        int j = i - 1;
        while (j >= 0 && ( (*comparacoes)++, strcmp(mochila[j].tipo, key.tipo) > 0)) {
            mochila[j + 1] = mochila[j];
            j--;
        }
        mochila[j + 1] = key;
    }
}

// SELECTION SORT (PRIORIDADE)
void selectionSort(Item mochila[], int qtd, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < qtd - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < qtd; j++) {
            (*comparacoes)++;
            if (mochila[j].prioridade < mochila[minIndex].prioridade) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Item tmp = mochila[i];
            mochila[i] = mochila[minIndex];
            mochila[minIndex] = tmp;
        }
    }
}

// BUSCA BINÁRIA POR NOME
int buscaBinaria(Item mochila[], int qtd, const char *nome) {
    int left = 0, right = qtd - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(mochila[mid].nome, nome);
        if (cmp == 0)
            return mid;
        else if (cmp < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

int main() {
    Item mochila[MAX];
    int qtd = 0;
    int opcao;
    int ordenadoPorNome = 0;
    char buffer[20];

    printf("\n===== PLANO DE FUGA =====\n");

    do {
        printf("\nMenu:\n");
        printf("1 - Adicionar componente\n");
        printf("2 - Listar componentes\n");
        printf("3 - Ordenar componentes\n");
        printf("4 - Buscar componente-chave (binária por nome)\n");
        printf("0 - Ativar Torre de Fuga (sair)\n");
        printf("Escolha: ");

        // Ler opção do menu
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &opcao) != 1) opcao = -1;

        switch(opcao) {
            case 1:
                if (qtd < MAX) {
                    // Nome
                    printf("Nome do componente: ");
                    fgets(mochila[qtd].nome, 30, stdin);
                    mochila[qtd].nome[strcspn(mochila[qtd].nome, "\n")] = '\0';

                    // Tipo
                    int tipoOpcao = 0;
                    while (tipoOpcao < 1 || tipoOpcao > 3) {
                        printf("Escolha o tipo do componente:\n");
                        printf("1 - estrutural\n");
                        printf("2 - eletronico\n");
                        printf("3 - energia\n");
                        printf("Opcao: ");
                        fgets(buffer, sizeof(buffer), stdin);
                        if (sscanf(buffer, "%d", &tipoOpcao) != 1) tipoOpcao = 0;

                        switch(tipoOpcao) {
                            case 1: strcpy(mochila[qtd].tipo, "estrutural"); break;
                            case 2: strcpy(mochila[qtd].tipo, "eletronico"); break;
                            case 3: strcpy(mochila[qtd].tipo, "energia"); break;
                            default: printf("Opcao invalida. Tente novamente.\n");
                        }
                    }

                    // Prioridade
                    int prioridade = 0;
                    while (prioridade < 1 || prioridade > 5) {
                        printf("Prioridade de montagem (1 a 5): ");
                        fgets(buffer, sizeof(buffer), stdin);
                        if (sscanf(buffer, "%d", &prioridade) != 1) prioridade = 0;
                    }
                    mochila[qtd].prioridade = prioridade;

                    // Quantidade
                    int quantidade = 0;
                    while (quantidade < 1) {
                        printf("Quantidade do componente: ");
                        fgets(buffer, sizeof(buffer), stdin);
                        if (sscanf(buffer, "%d", &quantidade) != 1) quantidade = 0;
                    }
                    mochila[qtd].quantidade = quantidade;

                    qtd++;
                    ordenadoPorNome = 0;
                } else {
                    printf("Mochila cheia!\n");
                }
                break;

            case 2:
                listarComponentes(mochila, qtd);
                break;

            case 3: {
                int tipo;
                printf("\nEscolha tipo de ordenação:\n");
                printf("1 - por Nome\n");
                printf("2 - por Tipo\n");
                printf("3 - por Prioridade\n");
                printf("Escolha: ");
                fgets(buffer, sizeof(buffer), stdin);
                if (sscanf(buffer, "%d", &tipo) != 1) tipo = 0;

                long comparacoes;
                clock_t ini, fim;
                double tempo;

                switch(tipo) {
                    case 1:
                        ini = clock();
                        bubbleSort(mochila, qtd, &comparacoes);
                        fim = clock();
                        tempo = (double)(fim - ini) / CLOCKS_PER_SEC;
                        printf("ordenação por nome concluído.\nComparações: %ld\nTempo: %.6f s\n", comparacoes, tempo);
                        ordenadoPorNome = 1;
                        break;
                    case 2:
                        ini = clock();
                        insertionSort(mochila, qtd, &comparacoes);
                        fim = clock();
                        tempo = (double)(fim - ini) / CLOCKS_PER_SEC;
                        printf("ordenação por tipo concluído.\nComparações: %ld\nTempo: %.6f s\n", comparacoes, tempo);
                        ordenadoPorNome = 0;
                        break;
                    case 3:
                        ini = clock();
                        selectionSort(mochila, qtd, &comparacoes);
                        fim = clock();
                        tempo = (double)(fim - ini) / CLOCKS_PER_SEC;
                        printf("ordenacao por prioridade concluída.\nComparações: %ld\nTempo: %.6f s\n", comparacoes, tempo);
                        ordenadoPorNome = 0;
                        break;
                    default:
                        printf("Opção inválida!\n");
                }
                listarComponentes(mochila, qtd);
                break;
            }

            case 4:
                if (!ordenadoPorNome) {
                    printf("É necessário ordenar por nome antes (opção 3).\n");
                } else {
                    char nome[30];
                    printf("Digite o nome do componente-chave: ");
                    fgets(nome, 30, stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    int idx = buscaBinaria(mochila, qtd, nome);
                    if (idx >= 0) {
                        printf("Componente-chave encontrado na posição %d:\n", idx + 1);
                        printf("Nome: %s | Tipo: %s | Prioridade: %d | Quantidade: %d\n",
                               mochila[idx].nome,
                               mochila[idx].tipo,
                               mochila[idx].prioridade,
                               mochila[idx].quantidade);
                    } else {
                        printf("Componente-chave não encontrado.\n");
                    }
                }
                break;

            case 0:
                printf("Ativando Torre de Fuga...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while(opcao != 0);

    return 0;
}
