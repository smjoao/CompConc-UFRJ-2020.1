#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int larg, alt;          // Largura e altura da imagem em pixels
unsigned char *ent;     // Matriz de pixels da imagem de entrada
unsigned char *saida;   // Matriz de pixels da imagem de saída
//Cada pixel é representado por 3 chars,
//um para cada canal da imagem RGB

void inverso() {

    // Para cada char na matriz de pixels da entrada, coloca o inverso na saída
    // Para inverter o valor, subtraio do valor máximo de char (255) o valor original
    for(int i = 0; i < alt * larg * 3; i++) {
        saida[i] = 255 - ent[i];
    }
}

int main(int argc, char *argv[]) {
    double start, finish;   // Variáveis para contar o tempo de execução

    // Garante que os argumentos foram digitados
    // corretamente na linha de comando
    if(argc < 3) {
        printf("Digite: %s <Caminho para imagem da entrada> <Caminho para a imagem da saída>\n", argv[0]);
        return 1;
    }

    // Carrega a imagem de entrada na variável ent
    if((ent = stbi_load(argv[1], &larg, &alt, NULL, 3)) == NULL) {
        printf("Erro ao carregar imagem de entrada!\n");
        return 4;
    }
    printf("Imagem carregada!\nLargura: %dpx\nAltura: %dpx\n", larg, alt);

    // Aloca memória para a saida
    saida = (unsigned char *) malloc(alt * larg * 3);

    GET_TIME(start);

    // Aplica o efeito inverso na imagem
    inverso();

    GET_TIME(finish);

    // Escreve a imagem de saida usando o caminho digitado na linha de comando
    if(stbi_write_png(argv[2], larg, alt, 3, saida, larg * 3) == 0) {
        printf("Erro ao escrever imagem de saída!\n");
        return 5;
    }
    printf("Imagem Salva!\n");

    // Imprime o tempo para aplicar o efeito inverso na imagem
    printf("Tempo para aplicar o efeito: %lf\n",finish - start);
    
    return 0;
}