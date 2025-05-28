#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
void handle_error(const char* mesaj)
{
    perror(mesaj);
    exit(2);
}
int main()
{
    int pipe1[2],pipe2[2],fifo[2];
    int pid1,pid2;
    if(-1 == pipe(pipe1))
        handle_error("Eroare la crearea primului pipe");
    if(-1 == pipe(pipe2))
        handle_error("Eroare la crearea celui de-al doilea pipe");
    if(-1 == mkfifo("canal_fifo",0666))
        handle_error("Eroare la crearea celui de-al treilea pipe");
    pid1 = fork();
    if(pid1==-1)
        handle_error("Eroare la primul fork");
    if(pid1==0)//primul fiu
    {
        if(-1 == dup2(pipe1[0],0))
            handle_error("EROARE LA DUP2");
        fifo[1] = open("canal_fifo",O_WRONLY);
        char c;
        while(read(0,&c,1)>0)
        {
            if(c>='0' && c<='9')
                write(fifo[1],&c,1);
            else if (c>='a' && c<='f')
                write(fifo[1],&c,1);
            else if (c>='A' && c<='F')
            {
                int dif = 'a'-'A';
                c = c + dif;
                write(fifo[1],&c,1);
            }
        }   
        close(fifo[1]);
        return 0;
    }
    pid2 = fork();
    if(pid2==-1)
        handle_error("Eroare la fork");
    if(pid2==0)
    {
        fifo[0] = open("canal_fifo",O_RDONLY);
        int fq[505];
        for(int i=0;i<=500;i++)
            fq[i] = 0;
        char c;
        int cnt = 0;
        while(read(fifo[0],&c,1)>0)
        {
            if(fq[c]==0)
                cnt++;
            fq[c]++;
        }
        int fd = open("statistics.txt",O_WRONLY|O_TRUNC|O_CREAT,0666);
        if(-1 == dup2(pipe2[1],1))
            handle_error("eroare la dup2 in al doilea fiu");
        for(int i=0;i<=500;i++)
        {
            if(fq[i]==0)
                continue;
            char c = (char)i;
            write(fd,&c,1);
            write(fd," ",1);
            int nr = fq[i];
            write(fd,&nr,sizeof(nr));
            write(fd,"\n",1);
        }
        write(1,&cnt,sizeof(cnt));
        close(pipe2[1]);
    }
    int fisier = open("input-data.txt",O_RDONLY);
    if(fisier==-1)
        handle_error("EROARE LA DESCHIDEREA FISIERULUI");
    if(-1 == dup2(pipe1[1],1))
        handle_error("eroare la dup2");
    char aux;
    while(read(fisier,&aux,1))
        write(1,&aux,1);
    if(-1 == dup2(pipe2[0],0))
        handle_error("eroare la dup2");
    int cnt;
    read(0,&cnt,sizeof(cnt));
    dup2(1,1);
    write(1,&cnt,sizeof(cnt));
}