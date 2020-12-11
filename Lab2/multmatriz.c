#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *mat1; // 1ª matriz
float *mat2; // 2ª matriz
float *res;  // Resultado

// Struct para os argumentos das threads
typedef struct {
    int id;
    int dim;
    int n_threads;
} t_Args;

// Função para tratar erros com o malloc
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("-- Erro no malloc --\n");
        exit(-1);
    }
    return ptr;
}

// Função realizada por cada thread
void *MultLinha(void *arg) {
    t_Args *args = (t_Args *) arg;

    printf("--- Thread %d iniciada ---\n", args->id);

    // Realiza a multiplicação para cada linha do resultado
    // intercalando as linhas para cada thread
    for(int i = args->id; i < args->dim; i += args->n_threads) {
        for(int j = 0; j < args->dim; j++) {
            for(int k = 0; k < args->dim; k++) {
                res[i*args->dim + j] += mat1[i*args->dim + k] * mat2[k*args->dim + j];
            }
        }
    }

    printf("--- Thread %d finalizada ---\n", args->id);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_t *tid;         // Identificadores das threads
    t_Args *args;           // Structs de argumentos para as threads
    int dim, n_threads;     // Dimensão da matriz e Nº de threads
    double start, finish;   // Variaveis para medir o tempo de execução

    if(argc < 3) {
        printf("Digite: ./multmatriz <dimensão das matrizes> <nº de threads>\n");
        return 0;
    }

    // Pega os argumentos da linha de comando
    dim = atoi(argv[1]);
    n_threads = atoi(argv[2]);

    // Limita n_threads para evitar criar threads desnecessarias
    if( n_threads > dim ) n_threads = dim;

    // Aloca espaço para as variaveis 
    tid  = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * n_threads);
    args = (t_Args *) errorcheck_malloc(sizeof(t_Args) * n_threads);
    mat1 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    mat2 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    res  = (float *) errorcheck_malloc(sizeof(float) * dim * dim);

    // Inicializa as matrizes
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            mat1[i*dim + j] = 2.0;
            mat2[i*dim + j] = 1.0;
            res[i*dim + j]  = 0.0;
        }
    }

    GET_TIME(start);

    // Inicia as threads
    for(int i = 0; i < n_threads; i++) {
        (args + i)->id = i;
        (args + i)->dim = dim;
        (args + i)->n_threads = n_threads;
        if(pthread_create(tid + i, NULL, MultLinha, (void *) (args + i))) {
            printf("-- Erro no pthread_create --\n");
            exit(-1);
        }
    }

    // Aguarda a finalização das threads
    for(int i = 0; i < n_threads; i++) {
        if(pthread_join(*(tid + i), NULL)) {
            printf("-- Erro no pthread_join --\n");
            exit(-1);
        }
    }
    GET_TIME(finish);
    printf("Tempo para criar as threads e executar a multiplicação: %f\n", finish - start);

    // Libera os espaços alocados
    free(tid);
    free(args);
    free(mat1);
    free(mat2);
    free(res);

    return 0;
}