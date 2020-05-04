#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#define NUM_CHILD 3

void sig_usr1(int signo){
    printf("%d : I received SIGUSR1(%d)\n\n",getpid(),SIGUSR1);
}

int main(){
    int i;
    pid_t child[NUM_CHILD];

    signal(SIGUSR1,(void*)sig_usr1);

    for(i=0;i<NUM_CHILD;i++){
        child[i] = fork();
        if(child[i] == 0)
            break;
    }

    pid_t pid_prev;
    if(i==0) pid_prev = getpid(); //if above fork error()
    else pid_prev = child[i-1];

    while(i-- > 0)
        sleep(1);
    
    printf("%d : I'm sending SIGUSR1(%d) to process %d\n",getpid(),SIGUSR1,pid_prev);
    kill(pid_prev,SIGINT);

    for(i=0;i<10;i++)
        sleep(1);
    
    return 0;
}