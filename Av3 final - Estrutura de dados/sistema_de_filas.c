#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. ESTRUTURA DO NÓ (Representa um documento na fila)
typedef struct Documento {
    int id;
    char usuario[50];
    char nome_arquivo[100];
    int qtd_paginas;
    struct Documento* proximo; // Ponteiro para o próximo documento da fila
} Documento;

// 2. ESTRUTURA DA FILA (Controla as extremidades Início e Fim)
typedef struct FilaImpressao {
    Documento* inicio;
    Documento* fim;
    int total_arquivos;
} FilaImpressao;

// Função para inicializar a fila
void inicializarFila(FilaImpressao* f) {
    f->inicio = NULL;
    f->fim = NULL;
    f->total_arquivos = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(FilaImpressao* f) {
    return (f->inicio == NULL);
}

// 3. OPERAÇÃO ENQUEUE (Inserir no Final da Fila)
void enviarParaSpool(FilaImpressao* f, int id, char* usuario, char* arquivo, int paginas) {
    // Aloca memória para o novo documento
    Documento* novo = (Documento*)malloc(sizeof(Documento));
    if (novo == NULL) {
        printf("\n[ERRO] Falha de memória ao alocar documento!\n");
        return;
    }

    // Copia os dados para o novo nó
    novo->id = id;
    strcpy(novo->usuario, usuario);
    strcpy(novo->nome_arquivo, arquivo);
    novo->qtd_paginas = paginas;
    novo->proximo = NULL; // Como ele vai para o fim, o próximo dele é NULL

    // Se a fila estiver vazia, o novo elemento será o início E o fim
    if (filaVazia(f)) {
        f->inicio = novo;
    } else {
        // Caso contrário, o atual fim aponta para o novo
        f->fim->proximo = novo;
    }
    
    // Atualiza o ponteiro de fim da fila para o novo elemento
    f->fim = novo;
    f->total_arquivos++;

    printf("\n[SUCESSO] Arquivo '%s' (%d págs) enviado ao spool por %s.\n", arquivo, paginas, usuario);
}

// 4. OPERAÇÃO DEQUEUE (Remover do Início da Fila / Simular Impressão)
void imprimirProximo(FilaImpressao* f) {
    if (filaVazia(f)) {
        printf("\n[AVISO] A fila de impressao esta vazia. Nada para processar.\n");
        return;
    }

    // Isola o nó que está no início
    Documento* aux = f->inicio;

    printf("\n--------------------------------------------------\n");
    printf("IMPRIMINDO AGORA:\n");
    printf("ID do Trabalho: #%d\n", aux->id);
    printf("Usuario:        %s\n", aux->usuario);
    printf("Arquivo:        %s\n", aux->nome_arquivo);
    printf("Paginas:        %d paginas\n", aux->qtd_paginas);
    printf("--------------------------------------------------\n");
    
    // Simulação do tempo de impressão baseado nas páginas (1 segundo a cada 5 páginas)
    int tempo_espera = (aux->qtd_paginas / 5) + 1;
    printf("Processando hardware da impressora... Aguarde %d segundos.\n", tempo_espera);
    ESPERAR(tempo_espera); 

    // Avança o início da fila para o próximo documento
    f->inicio = f->inicio->proximo;

    // Se a fila ficou vazia após a remoção, o fim também deve ser NULL
    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    // Libera a memória do documento impresso
    free(aux);
    f->total_arquivos--;

    printf("[CONCLUIDO] Trabalho de impressao finalizado com sucesso!\n");
}

// 5. OPERAÇÃO EXIBIR FILA (Percorrer a fila sem modificar)
void exibirFila(FilaImpressao* f) {
    if (filaVazia(f)) {
        printf("\n[FILA VAZIA] Nao ha trabalhos aguardando impressao.\n");
        return;
    }

    Documento* atual = f->inicio;
    int posicao = 1;
    int total_paginas_fila = 0;

    printf("\n============= FILA DE ESPERA ATUAL (%d arquivos) =============\n", f->total_arquivos);
    printf("%-5s | %-6s | %-15s | %-25s | %-8s\n", "Pos", "ID", "Usuario", "Arquivo", "Paginas");
    printf("----------------------------------------------------------------------\n");
    
    while (atual != NULL) {
        printf("%-5d | #%-5d | %-15s | %-25s | %-8d\n", 
               posicao, atual->id, atual->usuario, atual->nome_arquivo, atual->qtd_paginas);
        total_paginas_fila += atual->qtd_paginas;
        atual = atual->proximo;
        posicao++;
    }
    printf("----------------------------------------------------------------------\n");
    printf("Total de paginas acumuladas na fila: %d paginas.\n", total_paginas_fila);
    printf("======================================================================\n");
}

// 6. OPERAÇÃO LIMPAR FILA (Reset / Liberar toda memória restante)
void limparFila(FilaImpressao* f) {
    Documento* atual = f->inicio;
    while (atual != NULL) {
        Documento* prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    inicializarFila(f);
    printf("\n[INFO] Spool reiniciado. Todos os trabalhos pendentes foram cancelados.\n");
}

// PROGRAMA PRINCIPAL (Menu Interativo)
int main() {
    FilaImpressao fila;
    inicializarFila(&fila);
    
    int opcao;
    int gerador_id = 101; // IDs automáticos começando em 101

    // Variáveis auxiliares para leitura
    char user[50];
    char file[100];
    int pags;

    // Dados de teste pré-carregados para facilitar a apresentação pro professor
    enviarParaSpool(&fila, gerador_id++, "Ana_RH", "Relatorio_Salarios.pdf", 12);
    enviarParaSpool(&fila, gerador_id++, "Carlos_TI", "Script_Backup.txt", 3);
    enviarParaSpool(&fila, gerador_id++, "Diretoria", "Contrato_Final.docx", 45);

    do {
        printf("\n=============================================\n");
        printf("    SIMULADOR DE SPOOL DE IMPRESSAO (C)     \n");
        printf("=============================================\n");
        printf(" 1. Enviar arquivo para a fila (Enqueue)\n");
        printf(" 2. Processar proxima impressao (Dequeue)\n");
        printf(" 3. Visualizar fila de espera (Listar)\n");
        printf(" 4. Cancelar todos os trabalhos (Limpar)\n");
        printf(" 5. Sair do Sistema\n");
        printf("---------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o caractere '\n' do buffer do teclado

        switch(opcao) {
            case 1:
                printf("\n--- NOVO TRABALHO DE IMPRESSAO ---\n");
                printf("Nome do Usuario: ");
                fgets(user, 50, stdin);
                user[strcspn(user, "\n")] = 0; // Remove o '\n' lido pelo fgets

                printf("Nome do Arquivo: ");
                fgets(file, 100, stdin);
                file[strcspn(file, "\n")] = 0;

                printf("Quantidade de Paginas: ");
                scanf("%d", &pags);
                
                if (pags <= 0) {
                    printf("[ERRO] Quantidade de paginas invalida!\n");
                } else {
                    enviarParaSpool(&fila, gerador_id++, user, file, pags);
                }
                break;

            case 2:
                imprimirProximo(&fila);
                break;

            case 3:
                exibirFila(&fila);
                break;

            case 4:
                limparFila(&fila);
                break;

            case 5:
                printf("\nFechando o spooler. Liberando memoria...\n");
                limparFila(&fila); // Garante que nenhuma memória fique alocada ao sair
                printf("Sistema encerrado.\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
        }

    } while(opcao != 5);

    return 0;
}

