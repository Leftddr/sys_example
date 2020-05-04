#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

volatile int count = 0;
pthread_rwlock_t rwlock;
pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;

void* read_studio(void *);
void* write_studio(void *);

int main(int argc, char *argv[]){
    int ret = 0;
    pthread_mutex_lock(&a_mutex);
    count++;
    pthread_mutex_unlock(&a_mutex);
    /*
    ret = pthread_rwlock_init(&rwlock, NULL);
    pthread_rwlock_wrlock(&rwlock);
    count = count + 1;
    pthread_rwlock_unlock(&rwlock);
    /*
    if(ret != 0 ){printf("error\n"); return 0;}
    pthread_t readthr[3];
    pthread_t writethr;
    /*
    //ret = pthread_create(&readthr[0], NULL, read_studio, NULL);
    //if(ret != 0){printf("error\n"); return 0;}

    ret = pthread_create(&readthr[1], NULL, read_studio, NULL);
    if(ret != 0){printf("error\n"); return 0;}

    ret = pthread_create(&readthr[2], NULL, read_studio, NULL);
    if(ret != 0){printf("error\n"); return 0;}
    
    ret = pthread_create(&writethr, NULL, write_studio, NULL);
    if(ret != 0){printf("error\n"); return 0;}
    printf("%d\n", count);
    sleep(10);
    */

    //pthread_rwlock_destroy(&rwlock);
}

void* read_studio(void *arg){
    for(int i = 0 ; i < 10 ; i++){
        pthread_rwlock_rdlock(&rwlock);
        pthread_rwlock_unlock(&rwlock);
    }
}

void* write_studio(void *arg){
    
        pthread_rwlock_wrlock(&rwlock);
        count++;
        pthread_rwlock_unlock(&rwlock);
    
}