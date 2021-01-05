#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "timer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int n_threads;          // Nº de threads

int larg, alt;          // Largura e altura da imagem em pixels
unsigned char *ent;     // Matriz de pixels da imagem de entrada
unsigned char *saida;   // Matriz de pixels da imagem de saída
// Cada pixel é representado por 3 chars,
// um para cada canal da imagem RGB
// O nº de chars nas matrizes de pixels será larg * alt * 3

void *inverso(void *arg) {
    long int id = (long int) arg; // Pega o identificador da thread

    // Para cada char na matriz de pixels da entrada,
    // coloca o inverso na saída, alternando entre as threads
    // Para inverter o valor, subtraio do valor máximo de char (255) o valor original
    for(long int i = id; i < alt * larg * 3; i += n_threads) {
        saida[i] = 255 - ent[i];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t *tid;         // Identificadores das threads
    double start, finish;   // Variáveis para contar o tempo de execução

    // Garante que os argumentos foram digitados
    // corretamente na linha de comando
    if(argc < 4) {
        printf("Digite: %s <Caminho para imagem da entrada> <Caminho para a imagem da saída> <Nº de threads>\n", argv[0]);
        return 1;
    }

    // Pega o nº de threads da linha de comando
    n_threads = atoi(argv[3]);

    // Carrega a imagem de entrada na variável ent
    if((ent = stbi_load(argv[1], &larg, &alt, NULL, 3)) == NULL) {
        printf("Erro ao carregar imagem de entrada!\n");
        return 4;
    }
    printf("Imagem carregada!\nLargura: %dpx\nAltura: %dpx\n", larg, alt);

    // Limita n_threads para evitar criar threads desnecessarias
    if( n_threads > larg * alt * 3 ) n_threads = alt * larg * 3;

    // Aloca memória para os identificadores das threads
    tid = malloc(sizeof(pthread_t) * n_threads);

    // Aloca memória para a saida
    saida = (unsigned char *) malloc(alt * larg * 3);

    GET_TIME(start);
    
    // Cria as threads para aplicarem o efeito de inversão
    for(long int i = 0; i < n_threads; i++) {
        if(pthread_create(tid + i, NULL, inverso, (void *) i)) {
            printf("-- Erro no pthread_create --\n");
            return 2;
        }
    }
    
    // Aguarda todas as threads terminarem
    for(int i = 0; i < n_threads; i++) {
        if(pthread_join(*(tid + i), NULL)) {
            printf("-- Erro no pthread_join --\n");
            return 3;
        }
    }

    GET_TIME(finish);

    // Escreve a imagem de saida usando o caminho digitado na linha de comando
    if(stbi_write_jpg(argv[2], larg, alt, 3, saida, 100) == 0) {
        printf("Erro ao escrever imagem de saída!\n");
        return 5;
    }
    printf("Imagem Salva!\n");
    
    // Imprime o tempo para aplicar o efeito inverso na imagem
    printf("Tempo para aplicar o efeito: %lf\n",finish - start);

    return 0;
}