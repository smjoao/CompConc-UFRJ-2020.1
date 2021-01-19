#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define N 4 // Num de threads leitoras
#define M 3 // Num de threads escritoras

#define NUM 5 // Num de elementos no vetor vec

int *vec; // Vetor que sera manipulado pelos leitores e escritores

int leit = 0; // Num de threads leitoras operando
int esc = 0;  // Num de threads escritoras operando

// Variáveis de condião e mutex
pthread_mutex_t mutex;
pthread_cond_t condLeit, condEsc;

void EntraLeitura(int id) {
    pthread_mutex_lock(&mutex);
    printf("Leitor %d quer ler\n", id);
    while(esc > 0) {
        printf("Leitor %d bloqueado\n", id);
        pthread_cond_wait(&condLeit, &mutex);
        printf("Leitor %d desbloqueado\n", id);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

void SaiLeitura(int id) {
    pthread_mutex_lock(&mutex);
    printf("Leitor %d terminou leitura\n", id);
    leit--;
    if(leit == 0) pthread_cond_signal(&condEsc);
    pthread_mutex_unlock(&mutex);
}

void EntraEscrita(int id) {
    pthread_mutex_lock(&mutex);
    printf("Escritor %d quer escrever\n", id);
    while(leit > 0 || esc > 0) {
        printf("Escritor %d bloqueado\n", id);
        pthread_cond_wait(&condEsc, &mutex);
        printf("Escritor %d desbloqueado\n", id);
    }
    esc++;
    pthread_mutex_unlock(&mutex);

}

void SaiEscrita(int id) {
    pthread_mutex_lock(&mutex);
    printf("Escritor %d terminou escrita\n", id);
    esc--;
    pthread_cond_signal(&condEsc);
    pthread_cond_broadcast(&condLeit);
    pthread_mutex_unlock(&mutex);
}

void *Leitor(void *arg) {
    int id = (long int) arg;
    while(1) {
        EntraLeitura(id);
        double media = 0.0;
        char str[100], temp[50];
        sprintf(str, "Leitor %d: \"O conteúdo do vetor é ", id);
        for(int i = 0; i < NUM; i++) {
            media += vec[i]; 
            if(i == 0) sprintf(temp, "[%d", vec[i]);
            else sprintf(temp, ", %d", vec[i]);
            strcat(str, temp);
        }
        media /= NUM;
        sprintf(temp, "] e a média do vetor é %.1lf!\"\n", media);
        strcat(str, temp);
        printf("%s", str);
        SaiLeitura(id);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *Escritor(void *arg) {
    int id = (long int) arg;
    while(1) {
        EntraEscrita(id);
        printf("Escritor %d está escrevendo\n", id);
        vec[0] = id*2;
        vec[NUM-1] = id*2;
        for(int i = 1; i < NUM - 1; i++) {
            vec[i] = id;
        }
        SaiEscrita(id);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[M + N];

    // Inicializa o vetor que sera manipulado pelas threads
    vec = (int *) calloc(NUM, sizeof(int));

    // Inicializa o mutex e as variáveis de condição
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condLeit, NULL);
    pthread_cond_init(&condEsc, NULL);

    for(long int i = 0; i < N; i++) {
        if(pthread_create(&tid[i], NULL, Leitor, (void *) i)) exit(-1);
    }

    for(long int i = 0; i < M; i++) {
        if(pthread_create(&tid[N+i], NULL, Escritor, (void *) i)) exit(-1);
    }

    pthread_exit(NULL);
    return 0;
}