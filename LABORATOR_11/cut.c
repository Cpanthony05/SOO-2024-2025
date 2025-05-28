#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;
    if(-1 == (pid=fork()) )
    {
        perror("Eroare la fork");  exit(1);
    }

    if (pid == 0)
    {
        const char* args [] = {"cut","--fields","1,3,4,6","-d:","--output-delimiter=--","/etc/passwd",NULL};
        execvp("cut",args);
        perror("Eroare la exec");
        exit(10); 
    }
    wait(&status);
    if( WIFEXITED(status) )
    {
        switch( WEXITSTATUS(status) )
        {
            case 10:  printf("Comanda cut nu a putut fi executata... (apelul exec a esuat).\n");  break;
            case  0:  printf("Comanda cut a fost executata cu succes!\n");  break;
            default:  printf("Comanda cut a fost executata, dar a esuat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(status) );
        }
    }
    else
    {
        printf("Comanda cut a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(status) );
    }
    return 0;
}
