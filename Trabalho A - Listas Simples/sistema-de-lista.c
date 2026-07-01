#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <unistd.h>

#define MAX_MUSICAS 50

//=========================//
//      VARIÁVEIS          //
//=========================//
// Armazena a opção escolhida no menu principal.

int op = 0;
int totalMusicas = 0; // Controla a quantidade de músicas cadastradas.

//=========================//
//        ESTRUTURA        //
//=========================//
// Estrutura responsável por armazenar todas as
// informações de uma música da playlist.

typedef struct {
    int id;
    char titulo[100];
    char artista[100];
    char genero[50];
    char duracao[7];
} Musica;


Musica playlist[MAX_MUSICAS]; // Vetor que armazena todas as músicas cadastradas.

//=========================//
//      PROTÓTIPOS         //
//=========================//
// Declaração antecipada das funções do sistema.
void menu();
void cadastrarMusica();
void buscarMusica();
void alterarMusica();
void excluirMusica();
void listarMusicas();
void relatorio();

//=========================//
//   FUNÇÕES AUXILIARES    //
//=========================//
// Remove caracteres restantes do teclado,
// evitando problemas em leituras futuras.

void limparBuffer() {
    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
}

void limparTela() {
    system("clear");
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// Exibe um cabeçalho padronizado em todas as telas.
void cabecalho(char titulo[]) {
    limparTela();
    printf("=========================================\n");
    printf("          %s\n", titulo);
    printf("=========================================\n\n");
}

//=========================//
//     VALIDAÇÕES          //
//=========================//
// Verifica se o texto informado está vazio
// ou contém apenas espaços em branco.

bool campoVazio(char texto[]) {
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] != ' ' && texto[i] != '\n' && texto[i] != '\t')
            return false;
    }
    return true;
}

// Verifica se a duração foi digitada corretamente
// no formato MM:SS.
bool validarDuracao(char duracao[]) {
    if (strlen(duracao) != 5) return false;
    if (duracao[2] != ':') return false;

    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (duracao[i] < '0' || duracao[i] > '9')
            return false;
    }

    int segundos = (duracao[3] - '0') * 10 + (duracao[4] - '0');
    if (segundos > 59) return false;

    return true;
}

bool validarID(char idStr[]) {
    if (strlen(idStr) == 0) return false;

    for (int i = 0; idStr[i] != '\0'; i++) {
        if (idStr[i] < '0' || idStr[i] > '9')
            return false;
    }

    return true;
}

//=========================//
//     BUSCA POR ID        //
//=========================//
// Procura uma música pelo ID informado.
// Retorna a posição da música no vetor.
// Caso não encontre, retorna -1.
int buscarMusicaIndex(int id) {
    for (int i = 0; i < totalMusicas; i++) {
        if (playlist[i].id == id)
            return i;
    }
    return -1;
}
// Verifica se já existe uma música cadastrada
// com o ID informado.
bool existeMusica(int id) {
    return buscarMusicaIndex(id) != -1;
}

//=========================//
//        MENU             //
//=========================//

void menu() {
    cabecalho("PLAYLIST DE MÚSICAS");

    printf("1 - Cadastrar Música\n");
    printf("2 - Buscar Música\n");
    printf("3 - Alterar Música\n");
    printf("4 - Excluir Música\n");
    printf("5 - Listar Músicas\n");
    printf("6 - Relatório\n");
    printf("0 - Sair\n");

    printf("\nEscolha: ");
}

//=========================//
//      CADASTRAR          //
//=========================//
// Realiza o cadastro de uma nova música,
// validando todas as informações inseridas.

void cadastrarMusica() {
    cabecalho("CADASTRAR");
    
    // Verifica se ainda existe espaço disponível.
    if (totalMusicas >= MAX_MUSICAS) {
        printf("Limite atingido!\n");
        sleep(1);
        return;
    }

    // ID validado
    char idStr[20];

    do {
        printf("ID (somente números): ");
        fgets(idStr, 20, stdin);
        idStr[strcspn(idStr, "\n")] = '\0';

        if (!validarID(idStr))
            printf("❌ Apenas números (ex: 10)\n");

    } while (!validarID(idStr));

    int id = atoi(idStr);

    if (existeMusica(id)) {
        printf("ID já existe!\n");
        sleep(1);
        return;
    }

    playlist[totalMusicas].id = id;

    do {
        printf("Título: ");
        fgets(playlist[totalMusicas].titulo, 100, stdin);
        playlist[totalMusicas].titulo[strcspn(playlist[totalMusicas].titulo, "\n")] = '\0';
    } while (campoVazio(playlist[totalMusicas].titulo));

    do {
        printf("Artista: ");
        fgets(playlist[totalMusicas].artista, 100, stdin);
        playlist[totalMusicas].artista[strcspn(playlist[totalMusicas].artista, "\n")] = '\0';
    } while (campoVazio(playlist[totalMusicas].artista));

    do {
        printf("Gênero: ");
        fgets(playlist[totalMusicas].genero, 50, stdin);
        playlist[totalMusicas].genero[strcspn(playlist[totalMusicas].genero, "\n")] = '\0';
    } while (campoVazio(playlist[totalMusicas].genero));

    do {
        printf("Duração (MM:SS): ");
        fgets(playlist[totalMusicas].duracao, 7, stdin);
        playlist[totalMusicas].duracao[strcspn(playlist[totalMusicas].duracao, "\n")] = '\0';

        if (!validarDuracao(playlist[totalMusicas].duracao))
            printf("Formato inválido! Use MM:SS (ex: 03:45)\n");

    } while (!validarDuracao(playlist[totalMusicas].duracao));

    totalMusicas++;

    printf("\n✔ MÚSICA CADASTRADA!\n");
    sleep(2);
}

//=========================//
//        BUSCAR           //
//=========================//

void buscarMusica() {
    cabecalho("BUSCAR");

    char idStr[20];

    printf("ID: ");
    fgets(idStr, 20, stdin);
    idStr[strcspn(idStr, "\n")] = '\0';
    
    // Valida se o ID contém apenas números.
    if (!validarID(idStr)) {
        printf("ID inválido!\n");
        sleep(1);
        return;
    }

      // Converte o ID de texto para inteiro
    int id = atoi(idStr);
        
      // Procura a posição da música no vetor.
    int idx = buscarMusicaIndex(id);

    if (idx == -1) {
        printf("Não encontrada!\n");
        sleep(1);
    } else {
        printf("Título: %s\n", playlist[idx].titulo);
        printf("Artista: %s\n", playlist[idx].artista);
        printf("Gênero: %s\n", playlist[idx].genero);
        printf("Duração: %s\n", playlist[idx].duracao);
    }

    pausar();
}

//=========================//
//        ALTERAR          //
//=========================//
// Permite alterar todas as informações
// de uma música já cadastrada.

void alterarMusica() {
    cabecalho("ALTERAR");

    char idStr[20];

    printf("ID: ");
    fgets(idStr, 20, stdin);
    idStr[strcspn(idStr, "\n")] = '\0';

    if (!validarID(idStr)) {
        printf("ID inválido!\n");
        sleep(1);
        return;
    }

    int id = atoi(idStr);

    int idx = buscarMusicaIndex(id);

    if (idx == -1) {
        printf("Não encontrada!\n");
        sleep(1);
        return;
    }

    printf("Novo título: ");
    fgets(playlist[idx].titulo, 100, stdin);
    playlist[idx].titulo[strcspn(playlist[idx].titulo, "\n")] = '\0';

    printf("Novo artista: ");
    fgets(playlist[idx].artista, 100, stdin);
    playlist[idx].artista[strcspn(playlist[idx].artista, "\n")] = '\0';

    printf("Novo gênero: ");
    fgets(playlist[idx].genero, 50, stdin);
    playlist[idx].genero[strcspn(playlist[idx].genero, "\n")] = '\0';

    do {
        printf("Nova duração (MM:SS): ");
        fgets(playlist[idx].duracao, 7, stdin);
        playlist[idx].duracao[strcspn(playlist[idx].duracao, "\n")] = '\0';

        if (!validarDuracao(playlist[idx].duracao))
            printf("Formato inválido! Use MM:SS (ex: 03:45)\n");

    } while (!validarDuracao(playlist[idx].duracao));

    printf("\n✔ ALTERADO!\n");
    sleep(1);
    pausar();
}

//=========================//
//        EXCLUIR          //
//=========================//

void excluirMusica() {
    cabecalho("EXCLUIR");

    char idStr[20];

    printf("ID: ");
    fgets(idStr, 20, stdin);
    idStr[strcspn(idStr, "\n")] = '\0';

    if (!validarID(idStr)) {
        printf("ID inválido!\n");
        sleep(1);
        return;
    }

    int id = atoi(idStr);

    int idx = buscarMusicaIndex(id);

    if (idx == -1) {
        printf("Não encontrada!\n");
        sleep(1);
        return;
    }

    for (int i = idx; i < totalMusicas - 1; i++) {
        playlist[i] = playlist[i + 1];
    }

    totalMusicas--;

    printf("✔ EXCLUÍDA!\n");
    sleep(1);
    pausar();
}

//=========================//
//        LISTAR           //
//=========================//]
// Exibe todas as músicas cadastradas na playlist.

void listarMusicas() {
    cabecalho("LISTA");
    
    
    // Verifica se existe alguma música cadastrada.
    if (totalMusicas == 0) {
        printf("Nenhuma música cadastrada!\n");
        sleep(1);
        pausar();
        return;
    }

    for (int i = 0; i < totalMusicas; i++) {
        printf("ID: %d\n", playlist[i].id);
        printf("Título: %s\n", playlist[i].titulo);
        printf("Artista: %s\n", playlist[i].artista);
        printf("Gênero: %s\n", playlist[i].genero);
        printf("Duração: %s\n", playlist[i].duracao);
        printf("-------------------------\n");
    }

    pausar();
}

//=========================//
//       RELATÓRIO         //
//=========================//
// Exibe informações gerais da playlist.
void relatorio() {
    cabecalho("RELATÓRIO");

    printf("Total de músicas: %d\n", totalMusicas);

    sleep(1);
    pausar();
}

//=========================//
//          MAIN           //
//=========================//
// Inicia o sistema, exibe o menu principal e controla
// a execução das funcionalidades escolhidas pelo usuário.
int main() {
    setlocale(LC_ALL, "");

    do {
        menu();
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: cadastrarMusica(); break;
            case 2: buscarMusica(); break;
            case 3: alterarMusica(); break;
            case 4: excluirMusica(); break;
            case 5: listarMusicas(); break;
            case 6: relatorio(); break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                sleep(1);
        }

    } while (op != 0);

    return 0;
}