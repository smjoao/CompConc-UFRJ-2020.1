#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

// Variáveis globais
int x = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

// Thread 1
void *T1 (void *arg) {
    // Imprime a mensagem da thread
    printf("tudo bem?\n");

    // Inicia a seção crítica
    pthread_mutex_lock(&mutex);
    x++; // Incrementa x
    if( x == 2 ) {
        // Se as duas primeiras threads já tiverem terminado,
        // Desbloqueia as threads bloqueadas pela variável de condição
        pthread_cond_broadcast(&cond);
    }
    // Termina a seção crítica
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

// Thread 2
void *T2 (void *arg) {
    // Imprime a mensagem da thread
    printf("bom dia!\n");

    // Inicia a seção crítica
    pthread_mutex_lock(&mutex);
    x++; // Incrementa x
    if( x == 2 ) {
        // Se as duas primeiras threads já tiverem terminado,
        // Desbloqueia as threads bloqueadas pela variável de condição
        pthread_cond_broadcast(&cond);
    }
    // Termina a seção crítica
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

// Thread 3
void *T3 (void *arg) {

    // Inicia a seção crítica
    pthread_mutex_lock(&mutex);
    if( x < 2 ) {
        // Se as duas primeiras threads não tiverem terminado,
        // Bloquea essa thread com a variável de condição
        pthread_cond_wait(&cond, &mutex);
    }
    // Termina a seção crítica
    pthread_mutex_unlock(&mutex);
    
    // Imprime a mensagem da thread
    printf("até mais!\n");

    pthread_exit(NULL);
}

// Thread 4
void *T4 (void *arg) {
    
    // Inicia a seção crítica
    pthread_mutex_lock(&mutex);
    if( x < 2 ) {
        // Se as duas primeiras threads não tiverem terminado,
        // Bloquea essa thread com a variável de condição
        pthread_cond_wait(&cond, &mutex);
    }
    // Termina a seção crítica
    pthread_mutex_unlock(&mutex);
    
    // Imprime a mensagem da thread
    printf("boa tarde!\n");

    pthread_exit(NULL);
}

int main () {
    pthread_t tid[NTHREADS]; // Vetor com os identivicadores das threads

    // Inicializa as variaveis mutex e cond
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Cria e inicia as threads
    pthread_create(&tid[0], NULL, T1, NULL);
    pthread_create(&tid[1], NULL, T2, NULL);
    pthread_create(&tid[2], NULL, T3, NULL);
    pthread_create(&tid[3], NULL, T4, NULL);

    // Aguarda as threads terminarem
    for(int i = 0; i < NTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}