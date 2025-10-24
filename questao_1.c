#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int dia;
    int mes;
} Data;

typedef struct {
    char nome[41];
    char telefone[16];
    char celular[16];
    char email[41];
    Data dataAniversario;
} Contato;

struct elemento {
    Contato info;
    struct elemento* prox;
};
typedef struct elemento Elemento;

Elemento* cria_agenda();
Elemento* insere_contato(Elemento* lista);
void lista_contatos(Elemento* lista);
Elemento* busca_contato(Elemento* lista, char* nome);
Elemento* remove_contato(Elemento* lista, char* nome);
void atualiza_contato(Elemento* no); 

void exibir_menu();
void limpar_buffer();
int ler_string(char* str, int tamanho);
void free_agenda(Elemento* lista);
int validar_data(int dia, int mes);
void str_lower(char* dest, const char* src);
int campo_vazio(const char* str);
int ler_inteiro(const char* mensagem, int* valor);

int main() {
    Elemento* agenda = cria_agenda();
    int opcao;
    char nome_busca[41];
    Elemento* contato_encontrado = NULL;

    do {
        exibir_menu();
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
        }
        limpar_buffer(); 

        switch (opcao) {
            case 1:
                agenda = insere_contato(agenda);
                break;
            case 2:
                lista_contatos(agenda);
                break;
            case 3:
                printf("\n--- Buscar Contato ---\n");
                printf("Digite o nome para buscar: ");
                if (ler_string(nome_busca, 41) == 0) {
                    printf("Nome nao pode ser vazio.\n");
                    break;
                }
                
                contato_encontrado = busca_contato(agenda, nome_busca);
                
                if (contato_encontrado != NULL) {
                    printf("\n--- Contato Encontrado ---\n");
                    printf("Nome: %s\n", contato_encontrado->info.nome);
                    printf("Telefone: %s\n", contato_encontrado->info.telefone);
                    printf("Celular: %s\n", contato_encontrado->info.celular);
                    printf("Email: %s\n", contato_encontrado->info.email);
                    printf("Aniversario: %02d/%02d\n", contato_encontrado->info.dataAniversario.dia, contato_encontrado->info.dataAniversario.mes);
                } else {
                    printf("Contato '%s' nao encontrado.\n", nome_busca);
                }
                break;
            case 4:
                printf("\n--- Editar Contato ---\n");
                printf("Digite o nome do contato para editar: ");
                if (ler_string(nome_busca, 41) == 0) {
                    printf("Nome nao pode ser vazio.\n");
                    break;
                }
                
                contato_encontrado = busca_contato(agenda, nome_busca);
                
                if (contato_encontrado != NULL) {
                    atualiza_contato(contato_encontrado);
                } else {
                    printf("Erro: Contato '%s' nao encontrado para edicao.\n", nome_busca);
                }
                break;
            case 5:
                printf("\n--- Remover Contato ---\n");
                printf("Digite o nome do contato para remover: ");
                if (ler_string(nome_busca, 41) == 0) {
                    printf("Nome nao pode ser vazio.\n");
                    break;
                }
                
                agenda = remove_contato(agenda, nome_busca);
                break;
            case 6:
                printf("Saindo e liberando memoria...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

        if (opcao != 6) {
            printf("\nPressione Enter para continuar...");
            getchar(); 
        }

    } while (opcao != 6);

    free_agenda(agenda);

    return 0;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ler_string(char* str, int tamanho) {
    if (fgets(str, tamanho, stdin) != NULL) {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        } else {
            limpar_buffer();
        }
        return !campo_vazio(str);
    }
    return 0;
}

int ler_inteiro(const char* mensagem, int* valor) {
    printf("%s", mensagem);
    if (scanf("%d", valor) != 1) {
        limpar_buffer();
        return 0;
    }
    limpar_buffer();
    return 1;
}

void exibir_menu() {
    printf("\n=============================\n");
    printf("   AGENDA TELEFONICA\n");
    printf("=============================\n");
    printf("1. Inserir Contato\n");
    printf("2. Listar Contatos\n");
    printf("3. Buscar Contato\n");
    printf("4. Editar Contato\n");
    printf("5. Remover Contato\n");
    printf("6. Sair\n");
    printf("=============================\n");
    printf("Escolha uma opcao: ");
}

void free_agenda(Elemento* lista) {
    Elemento* p = lista;
    Elemento* temp;
    while (p != NULL) {
        temp = p;
        p = p->prox;
        free(temp);
    }
}

Elemento* cria_agenda() {
    return NULL;
}

int validar_data(int dia, int mes) {
    if (mes < 1 || mes > 12) {
        return 0;
    }
    if (dia < 1 || dia > 31) {
        return 0;
    }
    int dias_por_mes[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (dia > dias_por_mes[mes - 1]) {
        return 0;
    }
    return 1;
}

void str_lower(char* dest, const char* src) {
    int i = 0;
    while (src[i]) {
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

int campo_vazio(const char* str) {
    if (str == NULL || str[0] == '\0') {
        return 1;
    }
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

Elemento* insere_contato(Elemento* lista) {
    Elemento* novo_no = (Elemento*) malloc(sizeof(Elemento));
    if (novo_no == NULL) {
        printf("Erro: Falha na alocacao de memoria.\n");
        return lista; 
    }

    printf("\n--- Cadastro de Novo Contato ---\n");
    
    do {
        printf("Nome: ");
        if (ler_string(novo_no->info.nome, 41) == 0) {
            printf("Erro: Nome nao pode ser vazio. Tente novamente.\n");
        } else {
            break;
        }
    } while (1);

    printf("Telefone: ");
    ler_string(novo_no->info.telefone, 16);
    
    printf("Celular: ");
    ler_string(novo_no->info.celular, 16);
    
    printf("Email: ");
    ler_string(novo_no->info.email, 41);
    
    int dia, mes;
    do {
        if (!ler_inteiro("Dia de Aniversario (1-31): ", &dia) ||
            !ler_inteiro("Mes de Aniversario (1-12): ", &mes)) {
            printf("Erro: Digite numeros validos.\n");
            continue;
        }
        
        if (validar_data(dia, mes)) {
            novo_no->info.dataAniversario.dia = dia;
            novo_no->info.dataAniversario.mes = mes;
            break;
        } else {
            printf("Erro: Data invalida. Tente novamente.\n");
        }
    } while (1);

    novo_no->prox = NULL; 

    if (lista == NULL) {
        printf("Contato inserido com sucesso!\n");
        return novo_no;
    } else {
        Elemento* p = lista;
        while (p->prox != NULL) {
            p = p->prox;
        }
        p->prox = novo_no;
        printf("Contato inserido com sucesso!\n");
        return lista; 
    }
}

void lista_contatos(Elemento* lista) {
    if (lista == NULL) {
        printf("\nAgenda de contatos esta vazia.\n");
        return;
    }

    printf("\n========== LISTA DE CONTATOS ==========\n");
    Elemento* p = lista;
    int contador = 1;
    while (p != NULL) {
        printf("\n--- Contato %d ---\n", contador++);
        printf("Nome: %s\n", p->info.nome);
        printf("Telefone: %s\n", campo_vazio(p->info.telefone) ? "(nao informado)" : p->info.telefone);
        printf("Celular: %s\n", campo_vazio(p->info.celular) ? "(nao informado)" : p->info.celular);
        printf("Email: %s\n", campo_vazio(p->info.email) ? "(nao informado)" : p->info.email);
        printf("Aniversario: %02d/%02d\n", p->info.dataAniversario.dia, p->info.dataAniversario.mes);
        p = p->prox;
    }
    printf("\n=======================================\n");
}

Elemento* busca_contato(Elemento* lista, char* nome) {
    if (lista == NULL || campo_vazio(nome)) {
        return NULL;
    }
    
    char nome_busca_lower[41];
    char nome_lista_lower[41];
    str_lower(nome_busca_lower, nome);
    
    Elemento* p = lista;
    while (p != NULL) {
        str_lower(nome_lista_lower, p->info.nome);
        if (strcmp(nome_lista_lower, nome_busca_lower) == 0) {
            return p;
        }
        p = p->prox;
    }
    return NULL;
}

Elemento* remove_contato(Elemento* lista, char* nome) {
    if (lista == NULL) {
        printf("Erro: Agenda vazia.\n");
        return lista;
    }

    Elemento* ant = NULL; 
    Elemento* p = lista;
    
    char nome_busca_lower[41];
    char nome_lista_lower[41];
    str_lower(nome_busca_lower, nome);

    while (p != NULL) {
        str_lower(nome_lista_lower, p->info.nome);
        if (strcmp(nome_lista_lower, nome_busca_lower) == 0) {
            if (ant == NULL) {
                lista = p->prox;
            } else {
                ant->prox = p->prox;
            }
            printf("Contato '%s' removido com sucesso.\n", p->info.nome);
            free(p);
            return lista;
        }
        ant = p;
        p = p->prox;
    }

    printf("Erro: Contato '%s' nao encontrado para remocao.\n", nome);
    return lista;
}

void atualiza_contato(Elemento* no) {
    printf("\n--- Atualizando Contato: %s ---\n", no->info.nome);
    printf("(Deixe em branco para manter o valor atual)\n\n");

    char temp[41];
    
    printf("Telefone (Atual: %s): ", no->info.telefone);
    if (ler_string(temp, 16) > 0) {
        strcpy(no->info.telefone, temp);
    }

    printf("Celular (Atual: %s): ", no->info.celular);
    if (ler_string(temp, 16) > 0) {
        strcpy(no->info.celular, temp);
    }

    printf("Email (Atual: %s): ", no->info.email);
    if (ler_string(temp, 41) > 0) {
        strcpy(no->info.email, temp);
    }

    printf("Aniversario (Atual: %02d/%02d)\n", no->info.dataAniversario.dia, no->info.dataAniversario.mes);
    
    int dia, mes;
    if (ler_inteiro("Novo Dia (0 para manter): ", &dia) && dia != 0) {
        if (ler_inteiro("Novo Mes: ", &mes)) {
            if (validar_data(dia, mes)) {
                no->info.dataAniversario.dia = dia;
                no->info.dataAniversario.mes = mes;
                printf("Data de aniversario atualizada!\n");
            } else {
                printf("Data invalida. Mantendo data anterior.\n");
            }
        }
    }

    printf("\nContato atualizado com sucesso!\n");
}