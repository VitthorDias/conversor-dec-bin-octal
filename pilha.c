/******************************************************************************

Algoritmo para converter um número Decimal digitado pelo usuário para
Binário e Octal.

*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

// Structs para o Binário.
typedef struct Binario {
    int valor;
    struct Binario *prox;
}Binario;

typedef struct pilhaBin {
    Binario *top;
}pilhaBin;


// Structs para o Octal.
typedef struct Octal {
    int valor;
    struct Octal *prox;
}Octal;

typedef struct pilhaOct {
    Octal *top;
}pilhaOct;


// Função para alocar memoria e salvar o resto da divisão por 2
void empilhaBin(pilhaBin *p, int num){
    Binario *ptr = (Binario*) malloc(sizeof(Binario));
    if (ptr == NULL){
        printf("\nErro na alocacao de memoria...");
        return;
    }

    ptr->valor = num; // Armazena o resto
    ptr->prox = p->top; // Armazena qual é a proxima estrutura
    p->top = ptr; // O que era o topo recebe o novo topo criada
}

// Função para alocar memoria e salvar o resto da divisão por 8
void empilhaOct(pilhaOct *p, int num){
    Octal *ptr = (Octal*) malloc(sizeof(Octal));
    if (ptr == NULL){
        printf("\nErro na alocacao de memoria...");
        return;
    }
    
    ptr->valor = num; // Armazena o resto
    ptr->prox = p->top; // Armazena qual é a proxima estrutura
    p->top = ptr; // O que era o topo recebe o novo topo criada
}

void binary(int decimal){ 
    pilhaBin *point = (pilhaBin *) malloc(sizeof(pilhaBin));
    point->top = NULL; // Inicia o topo da pilha como vazio.
    if (point == NULL){
        printf("\nErro na alocacao de memoria...");
        return;
    }

    // Empilha o binário do decimal digitado de forma dinamica.
    while (decimal > 0){
        empilhaBin(point, decimal % 2);
        
        decimal = decimal / 2;
    }

    // Desempilha liberando memória e mostra o resultado da conversão.
    while (1){
        Binario *ptr = point->top;

        // Condição de parada.
        if (ptr == NULL){
            break;
        }

        printf("%d", ptr->valor);

        point->top = ptr->prox;
        // ptr->prox = NULL;
        free(ptr);
    }

    printf("\n");
}

void octal(int decimal){
    pilhaOct *point = (pilhaOct *) malloc(sizeof(pilhaOct));
    point->top = NULL; // Inicia o topo da pilha como vazio.
    if (point == NULL){
        printf("\nErro na alocacao de memoria...");
        return;
    }

    // Empilha o octal do decimal digitado de forma dinamica.
    while (decimal > 0){
        empilhaOct(point, decimal % 8);

        decimal = decimal / 8;
    }

    // Desempilha liberando memória e mostra o resultado da conversão.
    while (1){
        Octal *ptr = point->top;

        // Condição de parada.
        if (ptr == NULL){
            break;
        }

        printf("%d", ptr->valor);
        
        point->top = ptr->prox;
        // ptr->prox = NULL;
        free(ptr);
    }
}

void main(){
    int num;
    
    printf("Numero a ser convertido: ");
    scanf("%d", &num);

    // Chama a função binary() para alocar memoria, liberar memoria e mostrar o resultado da conversão para binário.
    printf("\nConvertendo para binario...\n");
    binary(num);

    // Chama a função octal() para alocar memoria, liberar memoria e mostrar o resultado da conversão para octal.
    printf("\nConvertendo para octal...\n");
    octal(num);
}
