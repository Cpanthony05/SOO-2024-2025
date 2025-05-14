#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N=0, i,j;
    int v[5];
    pid_t pid;
    for(i = 1; i <= 4; i++)
    {
        if(-1 == (pid=fork()) )
        {
            perror("Eroare la fork");  exit(2);
        }
        if(0 == pid) //nivelul 2 al arboreului
        {
            int w[5];
            for(j=1;j<=3;j++)
            {
                pid_t pid2;
                if(-1 == (pid2=fork()) )
                {
                    perror("Eroare la fork");  exit(2);
                }   
                if(0==pid2) // nivelul 3 al arborelui - nu ar fii.
                {
                    printf("Sunt procesul cu numarul in arbore (3,%d),cu pid-ul %d, iar parintele meu este procesul cu PID-ul: %d\n",(i-1)*4+j, getpid(), getppid() );
                    return j;
                }
                int status;
                waitpid(pid2,&status,0);
                if (WIFEXITED(status)) {
                    int exit_status = WEXITSTATUS(status);
                    w[j] = exit_status;
                } 
                else if (WIFSIGNALED(status))
                    w[j] = WTERMSIG(status);
            }
            printf("Sunt procesul cu numarul in arbore (2,%d),cu pid-ul %d, iar parintele meu este procesul cu PID-ul: %d.Fiii mei au avut urmatoarele coduri de terminare: ",i, getpid(), getppid() );       
            for(j=1;j<=3;j++)
                printf("%d ",w[j]);
            printf("\n");
            return i;  
        }
        int status;
        waitpid(pid,&status,0);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            v[i] = exit_status;
        } 
        else if (WIFSIGNALED(status))
            v[i] = WTERMSIG(status);
    }        
    printf("Sunt procesul cu numarul in arbore (1,1),cu pid-ul %d, iar parintele meu este procesul cu PID-ul: %d.Fiii mei au avut urmatoarele coduri de terminare: ", getpid(), getppid() );
    for(i=1;i<=4;i++)
        printf("%d ",v[i]);
    printf("\n");
    return 0;
}