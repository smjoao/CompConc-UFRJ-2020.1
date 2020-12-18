#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

long long int N;    // Número de termos
int n_threads;      // Número de threads

// Função para tratar erros com o malloc
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("-- Erro no malloc --\n");
        exit(-1);
    }
    return ptr;
}


/*
Para diminuir o erro numérico eu adiciono os termos 
de trás pra frente e separo os termos negativos e 
positivos para somá-los no final.
Foi o método que testei que obteve menor erro.
*/
double SomaPISeq(long long int N) {
    // Resultados das somas positivas e negativas
    double res1 = 0.0, res2 = 0.0; 

    // Soma os termos de trás pra frente, 
    // separando negativos e positivos
    for( long long int i = N - 1; i >= 0; i -= 1 ) {
        if(i%2) res1 -= 1.0/(2.0*i + 1.0);
        else    res2 += 1.0/(2.0*i + 1.0);
    }

    // Retorna a soma final
    return (res1 + res2) * 4;
}

/*
Usei a mesma técnica para diminuir o erro numérico
mas agora cada thread soma termos alternadamente 
*/
void *SomaPIConc(void *arg) {
    long int id = (long int) arg;   //Pega o id da thread
    double res1 = 0.0, res2 = 0.0;  //Resultados das somas positivas e negativas
    double *res;                    //Ponteiro para retornar o resultado final

    // Soma os termos de trás pra frente, 
    // separando negativos e positivos
    // intecalando os termos entre as threads
    for(long int i = N - id - 1; i >= 0; i -= n_threads) {
        if(i%2) res1 -= 1.0/(2.0*i + 1.0);
        else    res2 += 1.0/(2.0*i + 1.0);
    }

    // Coloca o resultado final no valor do ponteiro res
    res = (double *) errorcheck_malloc(sizeof(double));
    *res = 4.0 * (res1 + res2);

    // Retorna o resultado final usando o ponteiro res
    pthread_exit((void *) res);

}

int main(int argc, char *argv[]) {
    pthread_t *tid;         // Identificadores das threads
    double res;             // Resultado da soma
    double *ret;            // Ponteiro para pegar o retorno das threads
    double start, finish;   // Variáveis para contar o tempo de execução
    
    if(argc < 3) {
        printf("Digite: %s <Nº de elementos da soma> <Nº de threads>\n", argv[0]);
        return 1;
    }

    // Pega os parametros da linha de comando
    N = atoll(argv[1]);
    n_threads = atoll(argv[2]);

    // Limita n_threads para evitar criar threads desnecessarias
    if( n_threads > N ) n_threads = N;
    
    // Faz a soma sequencialmente, registrando o tempo
    GET_TIME(start);
    res = SomaPISeq(N);
    GET_TIME(finish);

    // Imprime resultados do programa sequencial
    printf("Programa sequencial:\nSoma de PI: %.15f\nErro da soma: %e\nTempo do programa: %f\n\n", res, M_PI - res, finish - start);

    // Faz a soma concorrente, registrando o tempo
    GET_TIME(start);
    tid = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * n_threads);
    for(long int i = 0; i < n_threads; i++) {
        if(pthread_create(tid+i, NULL, SomaPIConc, (void *) i)) {
            printf("-- Erro no pthread_create --\n");
            return 2;
        }
    }

    res = 0.0;
    for(long int i = 0; i < n_threads; i++) {
        if(pthread_join(*(tid + i), (void **) &ret)) {
            printf("-- Erro no pthread_join --\n");
            return 3;
        }
        res += *ret;
        free(ret);
    }
    GET_TIME(finish);

    // Imprime resultados do programa concorrente
    printf("Programa concorrente:\nSoma de PI: %.15f\nErro da soma: %e\nTempo do programa: %f\n\n", res, M_PI - res, finish - start);
}