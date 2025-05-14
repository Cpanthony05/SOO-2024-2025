#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
int fd;
int *M;
int N=0;
void handle_error(char* mesaj)
{
    perror(mesaj);
    exit (3);
}
void calcul_tata()
{
	for(int i=0;i<N;i++)
    {
        int x = M[2*i];
        int y = M[2*i+1];
        if(M[2*N+i]==0)
            printf("%d ",x+y);
        if(M[2*N+i]==1)
            printf("%d ",x-y);
        if(M[2*N+i]==2)
            printf("%d ",x*y);
        if(M[2*N+i]==3)
            printf("%d ",x/y);
    }
}
void generare_fiu1()
{
    for(int i=0;i<2*N;i++)
        M[i] = rand();
}
void generare_fiu2()
{
    for(int i=0;i<N;i++)
        M[2*N+i] = rand()%4;
}
int main()
{
	pid_t pid_fiu1, pid_fiu2;
	printf("Dati numarul intreg N:");
	scanf("%d", &N); 
    srand(time(NULL) + getpid());
    if(N<0)
        handle_error("N nu poate fi negativ!");
    fd = open("comunicare.txt",O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd<0)
        handle_error("Eroare la open");
    int dim = sizeof(int)*N*3;
    if(ftruncate(fd,dim)<0)
        handle_error("Eroare la ftruncate");
    M = mmap(NULL,dim,PROT_READ|PROT_WRITE,MAP_SHARED,fd);

	/* Crearea procesului fiu #1. */
	if(-1 == (pid_fiu1=fork()) )
	{
		perror("Eroare la fork #1");  return 1;
	}
	/* Ramificarea execuției după primul apel fork. */
	if(pid_fiu1 == 0)
	{   /* Zona de cod executată doar de către fiul #1. */
		printf("\n[P1] Procesul fiu 1, cu PID-ul: %d.\n", getpid());
		generare_fiu1();
		return 0;
	}
	else
	{   /* Zona de cod executată doar de către părinte. */
		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());
		/* Crearea procesului fiu #2. */
		if(-1 == (pid_fiu2=fork()) )
		{
			perror("Eroare la fork #2");  return 2;
		}
		/* Ramificarea execuției după al doilea apel fork. */
		if(pid_fiu2 == 0)
		{   /* Zona de cod executată doar de către fiul #2. */
			printf("\n[P2] Procesul fiu 2, cu PID-ul: %d.\n", getpid());
			generare_fiu2();
			return 0;
		}
		else
		{   /* Zona de cod executată doar de către părinte. */

			wait(NULL);
			wait(NULL);
			// Mai întâi aștept terminarea ambilor fii, iar apoi fac calculul.
			calcul_tata();
			return 0;
		}
	}
}



