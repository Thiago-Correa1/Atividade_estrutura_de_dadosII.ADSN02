#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>  

typedef struct Node {
    double value;       
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

Stack* createStack() {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    if (stack == NULL) {
        fprintf(stderr, "Erro: Falha na alocacao de memoria para a pilha.\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    return stack;
}

int isEmpty(Stack* stack) {
    return (stack->top == NULL);
}

void push(Stack* stack, double value) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Erro: Falha na alocacao de memoria para o no.\n");
        exit(EXIT_FAILURE);
    }
    newNode->value = value;
    newNode->next = stack->top;
    stack->top = newNode;
}

double pop(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Erro: Tentativa de pop em pilha vazia. Expressao malformada.\n");
        exit(EXIT_FAILURE);
    }
    
    Node* temp = stack->top;
    double value = temp->value;
    stack->top = temp->next;
    free(temp);
    
    return value;
}

void freeStack(Stack* stack) {
    Node* current = stack->top;
    Node* temp;
    
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    
    free(stack); 
}

int isOperator(const char* token) {
    return (strlen(token) == 1 && 
            (token[0] == '+' || token[0] == '-' || 
             token[0] == '*' || token[0] == '/'));
}

int isValidNumber(const char* token) {
    if (token == NULL || *token == '\0') {
        return 0;
    }
    
    char* endptr;
    strtod(token, &endptr);
    
    return (*endptr == '\0');
}

double evaluateRPN(char* expression) {
    Stack* stack = createStack();
    const char* delim = " "; 
    
    char* token = strtok(expression, delim);
    
    while (token != NULL) {
        if (isOperator(token)) {
            double val2, val1;
            
            if (isEmpty(stack)) {
                fprintf(stderr, "Erro: Expressao malformada (operandos insuficientes para o operador '%c').\n", token[0]);
                freeStack(stack);
                exit(EXIT_FAILURE);
            }
            val2 = pop(stack); 
            
            if (isEmpty(stack)) {
                fprintf(stderr, "Erro: Expressao malformada (operandos insuficientes para o operador '%c').\n", token[0]);
                freeStack(stack);
                exit(EXIT_FAILURE);
            }
            val1 = pop(stack); 
            
            double result;
            
            switch (token[0]) {
                case '+':
                    result = val1 + val2;
                    break;
                case '-':
                    result = val1 - val2; 
                    break;
                case '*':
                    result = val1 * val2;
                    break;
                case '/':
                    if (val2 == 0.0) {
                        fprintf(stderr, "Erro: Tentativa de divisao por zero.\n");
                        freeStack(stack);
                        exit(EXIT_FAILURE);
                    }
                    result = val1 / val2;
                    break;
                default:
                    fprintf(stderr, "Erro: Operador desconhecido '%c'.\n", token[0]);
                    freeStack(stack);
                    exit(EXIT_FAILURE);
            }
            
            push(stack, result);
            
        } else if (isValidNumber(token)) {
            push(stack, atof(token));
        } else {
            fprintf(stderr, "Erro: Token invalido '%s'.\n", token);
            freeStack(stack);
            exit(EXIT_FAILURE);
        }
        
        token = strtok(NULL, delim);
    }
    
    if (isEmpty(stack)) {
        fprintf(stderr, "Erro: Expressao vazia ou invalida.\n");
        freeStack(stack);
        exit(EXIT_FAILURE);
    }
    
    double finalResult = pop(stack);
    
    if (!isEmpty(stack)) {
        fprintf(stderr, "Erro: Expressao malformada (operandos em excesso).\n");
        freeStack(stack); 
        exit(EXIT_FAILURE);
    }
    
    freeStack(stack);
    
    return finalResult;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso correto: %s \"<expressao RPN>\"\n", argv[0]);
        fprintf(stderr, "Exemplo: %s \"5 1 2 + 4 * + 3 -\"\n", argv[0]);
        return 1; 
    }
    
    char* expression_copy = (char*) malloc(strlen(argv[1]) + 1);
    if (expression_copy == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memoria para a expressao.\n");
        return 1;
    }
    strcpy(expression_copy, argv[1]);
    
    double result = evaluateRPN(expression_copy);
    
    printf("Resultado: %.2f\n", result);
    
    free(expression_copy);
    
    return 0; 
}