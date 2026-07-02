#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Biblioteca para ler data e hora do sistema

#define MAX 100

// Definição do Registro
typedef struct {
    int id;
    char remetente[50];
    char assunto[100];
    char data[20];
} Email;

// modelo geral da pilha e controle de ID
Email pilha[MAX];
int topo = -1;
int proximoId = 1; //Variável global para controlar o autoincremento

//limpa buffer
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// Remove a nova linha do final da string
void removerNovaLinha(char* string) {
    size_t len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }
}

// Operações da Pilha

int isFull() {
    return topo == MAX - 1;
}

int isEmpty() {
    return topo == -1;
}

// PUSH: Receber e-mail (inserir no topo)
void push(Email e, int mostrarMensagem) {
    if (isFull()) {
        if (mostrarMensagem) printf("Erro: Bandeja de e-mails cheia (Stack Overflow)!\n");
        return;
    }
    pilha[++topo] = e;
    if (mostrarMensagem) printf("\n[+] E-mail '%s' recebido com sucesso no topo da bandeja!\n", e.assunto);
}

// POP: Ler e-mail do topo (remover do topo)
void pop() {
    if (isEmpty()) {
        printf("A bandeja de e-mails esta vazia.\n");
        return;
    }
    Email lido = pilha[topo--];
    printf("\n=== LENDO E-MAIL ===\n");
    printf("ID: %d | Data: %s\n", lido.id, lido.data);
    printf("De: %s\n", lido.remetente);
    printf("Assunto: %s\n", lido.assunto);
    printf("====================\n");
}

// PEEK: Consultar o topo sem remover
void peek() {
    if (isEmpty()) {
        printf("A bandeja de e-mails esta vazia.\n");
        return;
    }
    Email topoEmail = pilha[topo];
    printf("\n--- PROXIMO E-MAIL NA FILA DE LEITURA ---\n");
    printf("ID: %d | Data: %s\n", topoEmail.id, topoEmail.data);
    printf("Assunto: %s | De: %s\n", topoEmail.assunto, topoEmail.remetente);
    printf("-----------------------------------------\n");
}

// LISTAR: Do topo até a base
void listar() {
    if (isEmpty()) {
        printf("Bandeja vazia. Nenhum e-mail para listar.\n");
        return;
    }
    printf("\n--- BANDEJA DE E-MAILS (Topo para Base) ---\n");
    for (int i = topo; i >= 0; i--) {
        printf("[%d] De: %s | Assunto: %s | Data: %s\n", pilha[i].id, pilha[i].remetente, pilha[i].assunto, pilha[i].data);
    }
    printf("-------------------------------------------\n");
}

// Remover e-mail especifico com pilha auxiliar
void marcarComoLidoEspecifico(int idBusca) {
    if (isEmpty()) {
        printf("A bandeja esta vazia.\n");
        return;
    }

    Email auxPilha[MAX];
    int auxTopo = -1;
    int encontrado = 0;
    Email emailRemovido;

    while (!isEmpty()) {
        Email temp = pilha[topo--];
        if (temp.id == idBusca) {
            encontrado = 1;
            emailRemovido = temp;
            break; 
        } else {
            auxPilha[++auxTopo] = temp; 
        }
    }

    while (auxTopo >= 0) {
        pilha[++topo] = auxPilha[auxTopo--];
    }

    if (encontrado) {
        printf("\n[-] E-mail ID %d ('%s') marcado como lido e removido do meio da bandeja!\n", emailRemovido.id, emailRemovido.assunto);
    } else {
        printf("\nErro: E-mail com ID %d nao encontrado na bandeja.\n", idBusca);
    }
}

// Operações de Arquivo (CSV)

void salvarCSV() {
    FILE *arquivo = fopen("emails.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo emails.csv.\n");
        return;
    }

    fprintf(arquivo, "id;remetente;assunto;data\n");

    for (int i = 0; i <= topo; i++) {
        fprintf(arquivo, "%d;%s;%s;%s\n", pilha[i].id, pilha[i].remetente, pilha[i].assunto, pilha[i].data);
    }

    fclose(arquivo);
    printf("Dados salvos com sucesso em 'emails.csv'!\n");
}

void carregarCSV() {
    FILE *arquivo = fopen("emails.csv", "r");
    if (arquivo == NULL) {
        printf("Nenhum arquivo de dados encontrado. Iniciando nova bandeja...\n\n");
        return;
    }

    topo = -1; 
    char linha[256];

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        Email e;
        if (sscanf(linha, "%d;%49[^;];%99[^;];%19[^\n]", &e.id, e.remetente, e.assunto, e.data) == 4) {
            push(e, 0); 
        
            if (e.id >= proximoId) {
                proximoId = e.id + 1; 
            }
        }
    }

    fclose(arquivo);
    printf("Historico de e-mails carregado com sucesso!\n\n");
}

// Menu Principal
int main() {
    int opcao;
    
    carregarCSV(); 

    do {
        printf("============= BANDEJA DE E-MAILS (LIFO) =============\n");
        printf("1. Receber novo e-mail (Push)\n");
        printf("2. Ler e-mail do topo (Pop)\n");
        printf("3. Ver topo da bandeja (Peek)\n");
        printf("4. Listar todos os e-mails nao lidos\n");
        printf("5. Marcar um e-mail especifico como lido (ID)\n");
        printf("6. Salvar bandeja atual (CSV)\n");
        printf("0. Sair\n");
        printf("=====================================================\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) opcao = -1; 
        limparBuffer(); 

        switch (opcao) {
            case 1: {
                Email novo;
                printf("\n--- NOVO E-MAIL ---\n");
                
                novo.id = proximoId++;
                printf("ID Gerado: %d\n", novo.id);
                
                printf("Remetente: ");
                fgets(novo.remetente, sizeof(novo.remetente), stdin);
                removerNovaLinha(novo.remetente);
                
                printf("Assunto: ");
                fgets(novo.assunto, sizeof(novo.assunto), stdin);
                removerNovaLinha(novo.assunto);
                
                time_t t = time(NULL);
                struct tm *tm_info = localtime(&t);
                strftime(novo.data, sizeof(novo.data), "%d/%m/%Y %H:%M", tm_info);
                printf("Data capturada: %s\n", novo.data);

                push(novo, 1);
                break;
            }
            case 2:
                pop();
                break;
            case 3:
                peek();
                break;
            case 4:
                listar();
                break;
            case 5: {
                int idBusca;
                printf("\nDigite o ID do e-mail que deseja marcar como lido: ");
                scanf("%d", &idBusca);
                limparBuffer();
                marcarComoLidoEspecifico(idBusca);
                break;
            }
            case 6:
                salvarCSV();
                break;
            case 0:
                printf("\nSalvando antes de sair...\n");
                salvarCSV();
                printf("Encerrando programa...\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para voltar ao menu...");
            getchar(); // Espera a tecla Enter
            system("cls"); // Limpa o terminal no Windows
        }

    } while (opcao != 0);

    return 0;
}