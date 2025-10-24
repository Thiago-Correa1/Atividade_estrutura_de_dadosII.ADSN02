#include <stdio.h>
#include <stdlib.h>

int* cria_vetor(int n);
void imprime_vetor(int* vetor, int n);
void libera_vetor(int* vetor);

int main() {
    int n;
    int* meu_vetor;
    int i;
    
    printf("Digite o tamanho do vetor (n): ");
    scanf("%d", &n);
    
    /* Validacao de entrada */
    if (n <= 0) {
        printf("Erro: n deve ser maior que 0.\n");
        return 1;
    }
    
    meu_vetor = cria_vetor(n);
    
    printf("\nVetor criado com sucesso!\n");
    printf("Digite os %d elementos do vetor:\n", n);
    for (i = 0; i < n; i++) {
        printf("Elemento %d: ", i);
        scanf("%d", &meu_vetor[i]);
    }
    
    printf("\n--- Elementos do Vetor ---\n");
    imprime_vetor(meu_vetor, n);
    
    libera_vetor(meu_vetor);
    printf("\nMemoria liberada com sucesso.\n");
    
    return 0;
}

int* cria_vetor(int n) {
    int* vetor = (int*) malloc(n * sizeof(int));
    
    if (vetor == NULL) {
        printf("Erro: Falha na alocacao de memoria.\n");
        exit(1);
    }
    
    return vetor;
}

void imprime_vetor(int* vetor, int n) {
    int i;
    
    if (vetor == NULL) {
        printf("Erro: Ponteiro para vetor e nulo.\n");
        return;
    }
    
    for (i = 0; i < n; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

void libera_vetor(int* vetor) {
    if (vetor != NULL) {
        free(vetor);
    }
}