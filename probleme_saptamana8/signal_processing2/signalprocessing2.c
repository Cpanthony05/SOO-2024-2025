#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
int cnt = 0;
int N = 0;
int pid;
int fdw;
int primit;
void terminare()
{
    close(fdw);
    kill(pid,SIGUSR2);
    sigset_t mascatata;
    sigfillset(&mascatata);
    sigdelset(&mascatata,SIGCHLD);
    sigsuspend(&mascatata);
    printf("Parinte: sfarsit executie! \n");
}
void handle_sigalrm(int nr)
{
    signal(SIGALRM,handle_sigalrm);
    cnt++;
    if(cnt == 5 && !primit)
    {
        printf("Nu am primit input pentru %d secunde. Incheiam rularea procesului \n",cnt*N);
        terminare();
        exit(1);
    }
    if(!primit)
        printf("Introdu input! Au trecut %d secunde fara input \n",cnt*N);
}
void handle_sigusr2(int nr)
{
    return;
}
void handle_sigchld(int nr)
{
    return;
}
int main(int argc, char * argv[])
{
    argv[1] = "test.txt";
    argv[2] = "3";
    sigset_t mascagenerala;
    sigemptyset(&mascagenerala);
    sigaddset(&mascagenerala,SIGCHLD);
    sigaddset(&mascagenerala,SIGALRM);
    sigaddset(&mascagenerala,SIGUSR2);
    if(-1 == sigprocmask(SIG_BLOCK,&mascagenerala,NULL))
    {
        perror("Eroare la sigprocmask");
        exit(6);
    }
    N = atoi(argv[2]);
    signal(SIGUSR2,handle_sigusr2);
    signal(SIGALRM,handle_sigalrm);
    signal(SIGCHLD,handle_sigchld);
    if (-1 == (pid=fork()))
    {
        perror("Eroare la fork");
        exit(2);
    }
    if(pid!=0)
    {
        sigprocmask(SIG_UNBLOCK,&mascagenerala,NULL);
        fdw = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0666);
        if(fdw<0)
        {
            perror("Eroare la deschidere fisier output parinte");
            exit(11);
        }
        char c;
        int cnt = 0;
        for(int i=1;i<=5 && cnt==0;i++)
        {
            alarm(N);
            cnt = read(STDIN_FILENO,&c,sizeof(c));
            if(cnt > 0)
                break;
            if(cnt!=0 && errno!=EINTR)
                break;
            if(errno == EINTR)
            {
                cnt = 0;
            }
        }
        while(1)
        {
            if(cnt<0)
            {
                if(errno == EINTR)
                {
                    cnt = read(STDIN_FILENO,&c,sizeof(c));
                    continue;
                }
                perror("Eroare la citire in parinte");
                exit(10);
            }
            if(cnt==0)
                break;
            if(cnt>0)
            {
                primit = 1;
                cnt = write(fdw,&c,sizeof(c));
                if(cnt < 0)
                {
                    perror("Eroare la scriere in parinte");
                    exit(11);
                }
            }
            cnt = read(STDIN_FILENO,&c,sizeof(c));
        }

        terminare();
        return 0;

    }
    sigset_t mascafiu;
    sigfillset(&mascafiu);
    sigdelset(&mascafiu,SIGUSR2);
    sigsuspend(&mascafiu);

    printf("Fiul:Am primit USR2!\n");
    int fd;
    if(-1 == (fd = open(argv[1],O_RDONLY)))
    {
        perror("Fiu:eroare la deschidere fisier");
        exit(3);
    }
    char ch;
    int cod;
    do
    {
        cod = read(fd,&ch,sizeof(ch));
        if(cod==-1)
        {
            perror("Fiu:eroare la citire");
            exit(4);
        }
        printf("%c",ch);
    }while(cod!=0);
    printf("Fiul:Sfarsit executie!\n");
    return 0;
} 