/* 
Limitada a 5 digitos por limitação e o valor digitado tem está
entre 0 e 32767 devido a um overflow de mémoria no arduino.

Para confirmar o número digitado, Pressione o botão "#"
Para resetar o número digitado, Pressione o botão "*"
*/

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <stdlib.h>
#include <stdio.h>

#define length 5 

// Inicializa a lib passando os pinos
LiquidCrystal lcd (14, 15, 16, 17, 18, 19);

const byte LIN = 4; // Qtd de linhas
const byte COL = 4; // Qtd de colunas
int position = 0, pos = 0; // Posição no vetor
char numero[length]; // Vetor para salvar o decimal
unsigned int num; // Será responsavel em pegar o número convertido de string para inteiro

byte pinLIN[LIN] = {11, 10, 9, 8}; // Pinos das linhas teclado
byte pinCOL[COL] = {7, 6, 5, 4}; // Pinos das colunas teclado

char keys[LIN][COL] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Objeto de leitura das teclas com os parâmetros lidos
Keypad keypad = Keypad(makeKeymap(keys), pinLIN, pinCOL, LIN, COL);

// Structs para o Binário.
typedef struct Binario {
    unsigned int valor;
    struct Binario *prox;
}Binario;

typedef struct pilhaBin {
    Binario *top;
} pilhaBin;


// Structs para o Octal.
typedef struct Octal {
    unsigned int valor;
    struct Octal *prox;
}Octal;

typedef struct pilhaOct {
    Octal *top;
}pilhaOct;


void setup() {
  lcd.begin(16,2);
  lcd.clear();  
}

void loop() { 
  lcd.setCursor(0, 0);
  lcd.print("Digite o valor");
  
  char digito = keypad.getKey();
  
  switch (digito) {
    	// Confirma o número digitado
        case '#':
            num = atoi(numero); // Convert string para inteiro
    
            // Limitando a conversão para até 32767.
            if (num < 32768){
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Calculando");
              delay(500);

              // Faz a conversão para binario e print no lcd
              lcd.clear();
              lcd.print("Valor Binario");
              binary(num);
              delay(5000);

              // Faz a conversão para octal e print no lcd
              lcd.clear();
              lcd.print("Valor Octal");
              octal(num);
              delay(5000);
            }

            clearData();
            pos = 0; 
            lcd.clear();
            break;
        
    	// Irá resetar o número digitado
        case '*':
    		clearData();
            pos = 0;
            lcd.clear();
            break;
   		
    	// Ler os valores digitados
    	default:
    		// Caso digite "A" "B" "C" ou "D", o programa irá ignorar
            if (digito == 'A' || digito == 'B' || digito == 'C' || digito == 'D'){
              break;
            }
    		
    		// Salva o número digitado
            if (digito){
              numero[pos] = digito;    
              lcd.setCursor(pos, 1);
              lcd.print(numero[pos]);
              pos++;
   			 }
    		break;
    }
}

void clearData(){
  // Limpa o array para entrar com novos valores
  while (pos != 0){
    numero[pos--] = 0;
  }
}

// Função para alocar memoria e salvar o resto da divisão por 2
void empilhaBin( struct pilhaBin *p, int num){
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
void empilhaOct(struct pilhaOct *p, int num){
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
  	position = 0;
  
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
		
      	// Coloca cada valor em uma posição do LCD
      	lcd.setCursor(position++, 1);
        lcd.print(ptr->valor);

        point->top = ptr->prox;
        free(ptr);
    }
}

void octal(int decimal){
  	position = 0;
  	
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
		
      	// Coloca cada valor em uma posição do LCD
        lcd.setCursor(position++, 1); 
        lcd.print(ptr->valor);
        
        point->top = ptr->prox;
        free(ptr);
    }
}