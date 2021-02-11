#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
//#include<semaphore.h>

pthread_mutex_t em_e, em_l, escr, leit; //locks
//sem_t em_e, em_l, escr, leit; //semaforos
int e=0, l=0; //globais

int c = 0;

void *leitor(void *arg) {
    long int id = (long int) arg;
    while(1) {
        printf("Leitor[%ld] quer ler\n", id);
        pthread_mutex_lock(&leit);
        pthread_mutex_lock(&em_l); l++;
        if(l==1) pthread_mutex_lock(&escr);
        pthread_mutex_unlock(&em_l);
        pthread_mutex_unlock(&leit);
        printf("Leitor[%ld] começou a ler\n", id);
        sleep(1); // Simulando uma tarefa que demora algum tempo
        printf("%d\n", c);
        printf("Leitor[%ld] terminou de ler\n", id);
        pthread_mutex_lock(&em_l); l--;
        if(l==0) pthread_mutex_unlock(&escr);
        pthread_mutex_unlock(&em_l);
    }
}

void *escritor(void *arg) {
    long int id = (long int) arg;
    while(1) {
        printf("Escritor[%ld] quer escrever\n", id);
        pthread_mutex_lock(&em_e); e++;
        if(e==1) pthread_mutex_lock(&leit);
        pthread_mutex_unlock(&em_e);
        pthread_mutex_lock(&escr);
        printf("Escritor[%ld] começou a escrever\n", id);
        sleep(1); // Simulando uma tarefa que demora algum tempo
        c++;
        printf("Escritor[%ld] terminou de escrever\n", id);
        pthread_mutex_unlock(&escr);
        pthread_mutex_lock(&em_e); e--;
        if(e==0) pthread_mutex_unlock(&leit);
        pthread_mutex_unlock(&em_e);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t *escrs, *leits;
    int n_leit, n_escr; 
    long int i;

    if(argc < 3) {
        printf("Digite: %s <Nº de threads leitoras> <Nº de threads escritoras>\n", argv[0]);
        return 1;
    }

    //sem_init(&em_l, 0, 1);
    //sem_init(&em_e, 0, 1);
    //sem_init(&leit, 0, 1);
    //sem_init(&escr, 0, 1);

    // Nenhum dos locks é recursivo pois locks recursivos operam diferentemente de
    // semáforos. Se em uma das threads houvessem chamadas de sem_wait em sequencia
    // sem um sem_post intermediario a thread se bloquearia, isso não aconteceria com
    // locks recursivos, já que, se forem chamados locks em sequencia pela mesma thread ela
    // não se bloquearia.
    pthread_mutex_init(&em_l, NULL);
    pthread_mutex_init(&em_e, NULL);
    pthread_mutex_init(&leit, NULL);
    pthread_mutex_init(&escr, NULL);

    n_leit = atoi(argv[1]);
    n_escr = atoi(argv[2]);

    leits = (pthread_t *) malloc(sizeof(pthread_t) * n_leit);
    escrs = (pthread_t *) malloc(sizeof(pthread_t) * n_escr);

    for(i = 0; i < n_leit; i++) {
        pthread_create(leits + i, NULL, leitor, (void *) i);
    }

    for(i = 0; i < n_escr; i++) {
        pthread_create(escrs + i, NULL, escritor, (void *) i);
    }

    pthread_exit(NULL);
    return 0;
}
