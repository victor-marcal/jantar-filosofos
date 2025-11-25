#include <stdio.h>
#include <pthread.h>
#include <unistd.h> //função sleep
#include <stdlib.h> //função rand
#include <semaphore.h>

#define N 5 
#define LEFT (i+N-1)%N 
#define RIGHT (i+1)%N 
#define THINKING 0 
#define HUNGRY 1 
#define EATING 2 
int state[N]; //Estado de cada filosofo
sem_t mutex; 
sem_t s[N]; //Um semaforo por filosofo

void think_eat(int i, char action[]){
   int time = (rand() % 5 + 1);
   
   printf("Filósofo %d %s por %d segundos\n", i, action, time);
   sleep(time);  
} 

void test(int i){ 
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING; sem_post(&s[i]); 
    }
}

void take_forks(int i){ 
    sem_wait(&mutex); 
    state[i] = HUNGRY; 
    test(i); //Tenta adquirir os garfos
    sem_post(&mutex); 
    sem_wait(&s[i]); //Bloqueia caso não foi possível adquirir garfos
} 

void put_forks(int i){ 
    sem_wait(&mutex); 
    state[i] = THINKING; 
    //Verificando se os vizinhos podem comer
    test(LEFT); 
    test(RIGHT); 
    printf("Filósofo %d devolve o garfo\n", i);
    sem_post(&mutex); 
} 

void *philosopher (void *j){ 
    int i = *(int *) j;
    while (1) { 
        think_eat(i, "pensa"); 
        take_forks(i); //Pega os dois garfos na mesma função
        think_eat(i, "come"); 
        put_forks(i); //Devolve os garfos na mesma função tbm
        break;
    }
} 

int main(void){
    int i;
    int vet[] = {0,1,2,3,4};
    sem_init(&mutex,0,1);

    pthread_t philosophers[N];
    pthread_create(&philosophers[0], NULL, philosopher, (void *) &vet[0]);
    pthread_create(&philosophers[1], NULL, philosopher, (void *) &vet[1]);
    pthread_create(&philosophers[2], NULL, philosopher, (void *) &vet[2]);
    pthread_create(&philosophers[3], NULL, philosopher, (void *) &vet[3]);
    pthread_create(&philosophers[4], NULL, philosopher, (void *) &vet[4]);

    for(i=0; i<N; i++) {
        pthread_join(philosophers[i], NULL);
    }
    return 0;
}
