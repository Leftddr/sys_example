#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>

void sig_int(int signo);
void sig_usr(int signo);

int main(int argc, char *argv[]){
    struct sigaction oldsig,newsig;

    oldsig.sa_handler=sig_int;
    sigemptyset(&oldsig.sa_mask);
    oldsig.sa_flags=0;

    newsig.sa_handler=sig_usr;
    sigemptyset(&newsig.sa_mask);
    newsig.sa_flags=0;

    sigaction(SIGINT,&oldsig,0);
    sigaction(SIGUSR2,&newsig,0);

    sigset_t set,oldset;
    sigemptyset(&set);
    sigemptyset(&oldset);
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);

    sigprocmask(SIG_BLOCK,&set,NULL);
    printf("SIGINT AND SIGQUIT IS BLOCKED\n");
    
    sleep(5);

    sigdelset(&set,SIGINT);
    sigprocmask(SIG_UNBLOCK,&set,&oldset);

    set=oldset;

    sigprocmask(SIG_SETMASK,&set,NULL);
    sleep(5);

    sigprocmask(SIG_UNBLOCK,&set,NULL);

}