#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define N 10

// A thread recebe um ponteiro para o primeiro elemento do vetor que ela incrementará, 
// um inteiro indicando quantos elementos ele deve incrementar e um inteiro que identifica
// qual a thread executada
typedef struct {
    int *priElem;
    int NElem, idThread;
} t_Args;

void *IncVet(void *arg) {
    t_Args *args = (t_Args *) arg;
    int i;

    for( i = 0; i < args->NElem; i++ ) {
        (args->priElem[i])++;
    }
    printf("---Thread %d finalizada---\n\n", args->idThread);
    free(arg);


    pthread_exit(NULL);
}

int main() {
    pthread_t tid_sistema[2];   // Identificadores para as duas threads
    int vetor[N];               // Vetor que será incrementado
    int i, metadeVetor;         // Variáveis auxiliares
    t_Args *arg;                // Argumentos para as threads

    // Printa os valores iniciais do vetor
    printf("Valores iniciais do vetor:\n");
    for( i = 0; i < N; i++ ) {
        printf("%d\n", vetor[i]);
    }
    printf("\n");

    metadeVetor = N/2; // Divide o vetor em duas metades, cada thread trabalha em uma metade

    // Inicializa e atribui os argumentos da primeira thread
    arg = malloc(sizeof(t_Args));
    if(arg == NULL) exit(-1);
    arg->idThread = 1;
    arg->priElem = vetor;
    arg->NElem = metadeVetor;

    // Inicia a primeira thread
    printf("---Inicia a thread 1---\n\n");
    if (pthread_create(&tid_sistema[0], NULL, IncVet, (void*) arg)) exit(-1);

    // Inicializa e atribui os argumentos da segunda thread
    arg = malloc(sizeof(t_Args));
    if(arg == NULL) exit(-1);
    arg->idThread = 2;
    arg->priElem = &vetor[metadeVetor];
    arg->NElem = N - metadeVetor;

    // Inicia a segunda thread
    printf("---Inicia a thread 2---\n\n");
    if (pthread_create(&tid_sistema[1], NULL, IncVet, (void*) arg)) exit(-1);

    // Espera as duas threads finalizarem
    if (pthread_join(tid_sistema[0], NULL)) exit(-1);
    if (pthread_join(tid_sistema[1], NULL)) exit(-1);

    printf("---Todas as threads terminadas---\n\n");

    // Printa os valores finais do vetor
    printf("Valores finais do vetor:\n");
    for( i = 0; i < N; i++ ) {
        printf("%d\n", vetor[i]);
    }

    return 1;
}