#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// CONFIGURAÇÕES EXTRAS (Cores e Limpeza)
// ==========================================
#define COR_VERDE "\x1b[32m"
#define COR_VERMELHA "\x1b[31m"
#define COR_AMARELA "\x1b[33m"
#define COR_AZUL "\x1b[36m"
#define RESETAR_COR "\x1b[0m"

#ifdef _WIN32
#define LIMPAR_TELA "cls"
#else
#define LIMPAR_TELA "clear"
#endif

// ==========================================
// ESTRUTURAS DE DADOS DINÂMICAS
// ==========================================
typedef struct Trabalho
{
    int id;
    char nome_arquivo[100];
    int qtd_paginas;
    char tipo;
} Trabalho;

// Estrutura do Nó (A "caixa" que guarda o trabalho e aponta para o próximo)
typedef struct No
{
    Trabalho dados;
    struct No *proximo;
} No;

// 1. Fila Normal (Linear Simples Encadeada)
typedef struct FilaNormal
{
    No *inicio;
    No *fim;
    int tamanho;
} FilaNormal;

// 2. Fila Prioritária (Circular Simples Encadeada)
typedef struct FilaPrioritaria
{
    No *inicio;
    No *fim;
    int tamanho;
} FilaPrioritaria;

// Estrutura Extra: Estatísticas do Sistema
typedef struct Estatisticas
{
    int total_impressos_normal;
    int total_impressos_prioridade;
    int paginas_impressas;
} Estatisticas;

Estatisticas stats = {0, 0, 0}; // Variável global de estatísticas

// ==========================================
// INICIALIZAÇÃO E VALIDAÇÃO
// ==========================================
void inicializarFilas(FilaNormal *fn, FilaPrioritaria *fp)
{
    fn->inicio = NULL;
    fn->fim = NULL;
    fn->tamanho = 0;
    fp->inicio = NULL;
    fp->fim = NULL;
    fp->tamanho = 0;
}

int filaNormalVazia(FilaNormal *fn) { return fn->inicio == NULL; }
int filaPrioritariaVazia(FilaPrioritaria *fp) { return fp->inicio == NULL; }

// ==========================================
// CADASTRO / INSERÇÃO (ENQUEUE)
// ==========================================
void inserirTrabalho(FilaNormal *fn, FilaPrioritaria *fp, Trabalho t, int silencioso)
{
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL)
    {
        printf(COR_VERMELHA "\n[ERRO FATAL] Memoria RAM esgotada!\n" RESETAR_COR);
        return;
    }
    novo->dados = t;

    if (t.tipo == 'P' || t.tipo == 'p')
    {
        novo->dados.tipo = 'P';
        // Lógica Fila Circular
        if (filaPrioritariaVazia(fp))
        {
            fp->inicio = novo;
            fp->fim = novo;
            novo->proximo = fp->inicio; // Aponta para si mesmo (Circular)
        }
        else
        {
            fp->fim->proximo = novo;
            fp->fim = novo;
            novo->proximo = fp->inicio; // O último aponta de volta para o primeiro
        }
        fp->tamanho++;
        if (!silencioso)
            printf(COR_VERDE "\n[SUCESSO] Trabalho PRIORITARIO #%d adicionado!\n" RESETAR_COR, t.id);
    }
    else
    {
        novo->dados.tipo = 'N';
        novo->proximo = NULL; // Linear
        if (filaNormalVazia(fn))
        {
            fn->inicio = novo;
        }
        else
        {
            fn->fim->proximo = novo;
        }
        fn->fim = novo;
        fn->tamanho++;
        if (!silencioso)
            printf(COR_VERDE "\n[SUCESSO] Trabalho NORMAL #%d adicionado!\n" RESETAR_COR, t.id);
    }
}

// ==========================================
// PROCESSAMENTO (DEQUEUE)
// ==========================================
void processarImpressao(FilaNormal *fn, FilaPrioritaria *fp)
{
    if (!filaPrioritariaVazia(fp))
    {
        No *aux = fp->inicio;
        Trabalho t = aux->dados;

        // Se tem apenas 1 elemento
        if (fp->inicio == fp->fim)
        {
            fp->inicio = NULL;
            fp->fim = NULL;
        }
        else
        {
            fp->inicio = fp->inicio->proximo;
            fp->fim->proximo = fp->inicio; // Mantém a circularidade
        }
        free(aux);
        fp->tamanho--;
        stats.total_impressos_prioridade++;
        stats.paginas_impressas += t.qtd_paginas;

        printf(COR_AMARELA "\n---------------------------------------------\n");
        printf("[IMPRIMINDO] TRABALHO PRIORITARIO\n");
        printf("ID: #%d | Arquivo: %s | Paginas: %d\n", t.id, t.nome_arquivo, t.qtd_paginas);
        printf("---------------------------------------------\n" RESETAR_COR);
        return;
    }

    if (!filaNormalVazia(fn))
    {
        No *aux = fn->inicio;
        Trabalho t = aux->dados;

        fn->inicio = fn->inicio->proximo;
        if (fn->inicio == NULL)
        {
            fn->fim = NULL; // Se esvaziou, o fim também é nulo
        }
        free(aux);
        fn->tamanho--;
        stats.total_impressos_normal++;
        stats.paginas_impressas += t.qtd_paginas;

        printf(COR_AZUL "\n---------------------------------------------\n");
        printf("[IMPRIMINDO] TRABALHO NORMAL\n");
        printf("ID: #%d | Arquivo: %s | Paginas: %d\n", t.id, t.nome_arquivo, t.qtd_paginas);
        printf("---------------------------------------------\n" RESETAR_COR);
        return;
    }

    printf(COR_VERMELHA "\n[AVISO] Nao ha trabalhos na fila de impressao.\n" RESETAR_COR);
}

// ==========================================
// LISTAGEM E ESTATÍSTICAS
// ==========================================
void listarFilas(FilaNormal *fn, FilaPrioritaria *fp)
{
    printf("\n=================== STATUS DO SPOOL ===================\n");

    // Lista Fila Circular
    printf("--- FILA PRIORITARIA (CIRCULAR) [%d trabalhos] ---\n", fp->tamanho);
    if (filaPrioritariaVazia(fp))
    {
        printf(" [Vazia]\n");
    }
    else
    {
        No *atual = fp->inicio;
        int pos = 1;
        do
        {
            printf("  Pos %d -> ID: #%d | Arq: %-20s | Pags: %d\n", pos++, atual->dados.id, atual->dados.nome_arquivo, atual->dados.qtd_paginas);
            atual = atual->proximo;
        } while (atual != fp->inicio);
    }

    // Lista Fila Linear
    printf("\n--- FILA NORMAL (LINEAR) [%d trabalhos] ---\n", fn->tamanho);
    if (filaNormalVazia(fn))
    {
        printf(" [Vazia]\n");
    }
    else
    {
        No *atual = fn->inicio;
        int pos = 1;
        while (atual != NULL)
        {
            printf("  Pos %d -> ID: #%d | Arq: %-20s | Pags: %d\n", pos++, atual->dados.id, atual->dados.nome_arquivo, atual->dados.qtd_paginas);
            atual = atual->proximo;
        }
    }

    // Extras: Estatísticas
    printf("\n--- ESTATISTICAS DE HOJE ---\n");
    printf(" Normais Impressos: %d | Prioritarios Impressos: %d | Paginas Totais: %d\n",
           stats.total_impressos_normal, stats.total_impressos_prioridade, stats.paginas_impressas);
    printf("=======================================================\n");
}

// ==========================================
// CANCELAMENTO / REMOÇÃO
// ==========================================
void cancelarTrabalho(FilaNormal *fn, FilaPrioritaria *fp, int id)
{
    // Busca e Remove na Fila Normal (Linear)
    No *atual_N = fn->inicio;
    No *anterior_N = NULL;
    while (atual_N != NULL)
    {
        if (atual_N->dados.id == id)
        {
            if (anterior_N == NULL)
                fn->inicio = atual_N->proximo; // Era o primeiro
            else
                anterior_N->proximo = atual_N->proximo; // Estava no meio

            if (atual_N == fn->fim)
                fn->fim = anterior_N; // Era o último

            free(atual_N);
            fn->tamanho--;
            printf(COR_VERDE "\n[CANCELADO] Trabalho Normal #%d removido.\n" RESETAR_COR, id);
            return;
        }
        anterior_N = atual_N;
        atual_N = atual_N->proximo;
    }

    // Busca e Remove na Fila Prioritária (Circular)
    if (!filaPrioritariaVazia(fp))
    {
        No *atual_P = fp->inicio;
        No *anterior_P = fp->fim; // O anterior do início é o fim na circular
        do
        {
            if (atual_P->dados.id == id)
            {
                // Caso seja o único elemento
                if (atual_P == fp->inicio && atual_P == fp->fim)
                {
                    fp->inicio = NULL;
                    fp->fim = NULL;
                }
                else
                {
                    anterior_P->proximo = atual_P->proximo;
                    if (atual_P == fp->inicio)
                        fp->inicio = atual_P->proximo;
                    if (atual_P == fp->fim)
                        fp->fim = anterior_P;
                }
                free(atual_P);
                fp->tamanho--;
                printf(COR_VERDE "\n[CANCELADO] Trabalho Prioritario #%d removido.\n" RESETAR_COR, id);
                return;
            }
            anterior_P = atual_P;
            atual_P = atual_P->proximo;
        } while (atual_P != fp->inicio);
    }

    printf(COR_VERMELHA "\n[ERRO] Trabalho ID #%d nao encontrado.\n" RESETAR_COR, id);
}

// ==========================================
// PERSISTÊNCIA (CSV)
// ==========================================
void salvarCSV(FilaNormal *fn, FilaPrioritaria *fp)
{
    FILE *arq = fopen("spool_impressao.csv", "w");
    if (!arq)
        return;

    if (!filaPrioritariaVazia(fp))
    {
        No *atual = fp->inicio;
        do
        {
            fprintf(arq, "%d;%s;%d;%c\n", atual->dados.id, atual->dados.nome_arquivo, atual->dados.qtd_paginas, atual->dados.tipo);
            atual = atual->proximo;
        } while (atual != fp->inicio);
    }

    No *atual_N = fn->inicio;
    while (atual_N != NULL)
    {
        fprintf(arq, "%d;%s;%d;%c\n", atual_N->dados.id, atual_N->dados.nome_arquivo, atual_N->dados.qtd_paginas, atual_N->dados.tipo);
        atual_N = atual_N->proximo;
    }
    fclose(arq);
    printf(COR_VERDE "\n[SUCESSO] Dados salvos em CSV.\n" RESETAR_COR);
}

int carregarCSV(FilaNormal *fn, FilaPrioritaria *fp)
{
    FILE *arq = fopen("spool_impressao.csv", "r");
    if (!arq)
        return 100; // Começa no 101 se não houver arquivo

    Trabalho t;
    int maior_id = 100;

    // A flag "1" serve para adicionar de forma silenciosa (sem poluir o terminal ao iniciar)
    while (fscanf(arq, "%d;%[^;];%d;%c\n", &t.id, t.nome_arquivo, &t.qtd_paginas, &t.tipo) == 4)
    {
        if (t.id > maior_id)
            maior_id = t.id;
        inserirTrabalho(fn, fp, t, 1);
    }
    fclose(arq);
    return maior_id;
}

// ==========================================
// PROGRAMA PRINCIPAL & MENU INTERATIVO
// ==========================================
int main()
{
    FilaNormal fila_normal;
    FilaPrioritaria fila_prioritaria;
    inicializarFilas(&fila_normal, &fila_prioritaria);

    int gerador_id = carregarCSV(&fila_normal, &fila_prioritaria);
    int opcao, id_busca;
    Trabalho novo;

    do
    {
        printf("\n" COR_AZUL ">>> SISTEMA DE IMPRESSAO CORPORATIVO (DINAMICO) <<<" RESETAR_COR "\n");
        printf("1. Adicionar Trabalho (Enqueue)\n");
        printf("2. Imprimir Proximo (Dequeue c/ Prioridade)\n");
        printf("3. Status, Listagem e Relatorio\n");
        printf("4. Cancelar Trabalho por ID\n");
        printf("5. Limpar a Tela\n");
        printf("6. Sair e Salvar\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            printf(COR_VERMELHA "\n[ERRO] Digite um numero valido.\n" RESETAR_COR);
            while (getchar() != '\n')
                ;
            continue;
        }
        getchar();

        switch (opcao)
        {
        case 1:
            gerador_id++;
            novo.id = gerador_id;
            printf("Nome do Arquivo: ");
            fgets(novo.nome_arquivo, 100, stdin);
            novo.nome_arquivo[strcspn(novo.nome_arquivo, "\n")] = 0;

            do
            {
                printf("Quantidade de Paginas: ");
                scanf("%d", &novo.qtd_paginas);
                if (novo.qtd_paginas <= 0)
                    printf(COR_VERMELHA "Quantidade deve ser maior que zero.\n" RESETAR_COR);
            } while (novo.qtd_paginas <= 0);
            getchar();

            do
            {
                printf("Tipo (N - Normal / P - Prioritario): ");
                scanf("%c", &novo.tipo);
                getchar();
            } while (novo.tipo != 'N' && novo.tipo != 'n' && novo.tipo != 'P' && novo.tipo != 'p');

            inserirTrabalho(&fila_normal, &fila_prioritaria, novo, 0);
            break;

        case 2:
            processarImpressao(&fila_normal, &fila_prioritaria);
            break;
        case 3:
            listarFilas(&fila_normal, &fila_prioritaria);
            break;
        case 4:
            printf("ID do trabalho para cancelar: ");
            scanf("%d", &id_busca);
            cancelarTrabalho(&fila_normal, &fila_prioritaria, id_busca);
            break;
        case 5:
            system(LIMPAR_TELA);
            break;
        case 6:
            salvarCSV(&fila_normal, &fila_prioritaria);
            printf("\nEncerrando e liberando memoria...\n");
            break;
        default:
            printf(COR_VERMELHA "\n[ERRO] Opcao invalida!\n" RESETAR_COR);
        }
    } while (opcao != 6);

    return 0;
}