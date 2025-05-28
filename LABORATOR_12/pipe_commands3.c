#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
void handle_error(const char * mesaj)
{
    perror(mesaj);
    exit(2);
}
int main()
{
    int pid1,pid2,pid3;
    int pipe1[2],pipe2[2],pipe3[2];
    if(-1 == pipe(pipe1))
        handle_error("Eroare la crearea primului pipe");
    if(-1 == pipe(pipe2))
        handle_error("Eroare la crearea celui de-al doilea pipe");
    if(-1 == pipe(pipe3))
        handle_error("Eroare la crearea celui de-al treilea pipe");
    pid1 = fork();
    if(pid1==-1)
        handle_error("Eroare la primul fork");
    if(pid1==0)//primul fiu
    {
        if(-1==dup2(pipe1[1],1))
            handle_error("Eroare la dup in primul fiu");
        execlp("ps","ps","-eo","user,comm,pid","--no-headers",NULL);
        handle_error("Eroare la execlp pentru prima comanda din pipe");
    }
    pid2 = fork();
    if(pid2==-1)
        handle_error("eroare la al doilea fork");
    if(pid2==0)//al doilea fiu
    {
        close(pipe1[1]);
        if(-1==dup2(pipe2[1],1))
            handle_error("Eroare la dup in al doilea fiu");
        if(-1==dup2(pipe1[0],0))
            handle_error("Eroare la dup in al doilea fiu");
        execlp("tr","tr","-s"," ",NULL);
        handle_error("Eroare la execlp pentru a doua comanda din pipe");
    }
    pid3 = fork();
    if(pid3==-1)
        handle_error("eroare la al treile fork");
    if(pid3==0)//al treila fiu
    {
        close(pipe1[1]);
        close(pipe2[1]);
        if(-1==dup2(pipe3[1],1))
            handle_error("eroare la dup in al treilea fiu");
        if(-1==dup2(pipe2[0],0))
            handle_error("eroare la dup in al treila fiu");
        execlp("cut","cut","-d" "","-f1,2,3","--output-delimiter=:",NULL);
        handle_error("Eroare la execlp pentru a treia comanda din pipe");
    }
    //parintele
    close(pipe1[1]);
    close(pipe2[1]);
    close(pipe3[1]);
    if(-1==dup2(pipe3[0],0))
        handle_error("eroare la dup in parinte");
    execlp("sort","sort","-k2","-t:",NULL);
    handle_error("Eroare la execlp pentru ultima comanda din pipe");
	return 0;  // Fluxul de execuție nu va ajunge niciodată aici!
}
