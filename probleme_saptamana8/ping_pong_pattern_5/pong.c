#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
void USR_handler(int nr)
{
    signal(SIGUSR1,USR_handler);
    return;
}
void initializarepid()
{
    int pid = getpid();
    int fdpid = open("pid.txt",O_RDWR|O_TRUNC|O_CREAT,0666);
    write(fdpid,&pid,sizeof(pid));
    close(fdpid);
}
int main()
{
    initializarepid();
    sigset_t masca;
    sigfillset(&masca);
    sigdelset(&masca,SIGUSR1);
    signal(SIGUSR1,USR_handler);
    int fdr = open("fis.txt",O_RDONLY);
    if(fdr==-1)
    {
        perror("Eroare la deschidere fisier input");
        exit(1);
    }
    int fdw= open("fis_poz-pare.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
    if(fdw < 0)
    {
        perror("Eroare la deschiderea fisierului de scriere pozitii pare");
        exit(2);
    }
    int cnt = 0;
    char ch;
    int cod = 0;
    do
    {
        sigsuspend(&masca);
        printf("Procesul pong");
        cod = read(fdr,&ch,sizeof(ch));
        if(cod<0)
        {
            perror("Eroare la citire");
            exit(3);
        }
        if(cod>0)
        {
            cnt++;
            if(cnt%2==0)
            {
                int codw = write(fdw,&ch,sizeof(ch));
                if(codw<0)
                {
                    perror("Eroare la scriere");
                    exit(4);
                }
            }
        }
        int pid = getpid();
        int fdpid = open("pid.txt",O_RDWR|O_TRUNC);
        int targetid = read(fdpid,&targetid,sizeof(targetid));
        close(fdpid);

        fdpid = open("pid.txt",O_RDWR|O_TRUNC);
        write(fdpid,&pid,sizeof(pid));
        close(fdpid);

        kill(targetid,SIGUSR1);
    } while (cod!=0);
    return 0;
}