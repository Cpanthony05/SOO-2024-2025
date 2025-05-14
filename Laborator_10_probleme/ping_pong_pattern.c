#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
int zero = 0;
int unu = 1;
void handle_error(char* mesaj)
{
    perror(mesaj);
    exit(2);
}
void dialog_tata()
{
    int fd = open("flag.bin",O_RDWR|O_CREAT,0666);
    if(fd<0)
        handle_error("Eroare la open");
    int x = -1;
	while(x!=zero)
    {
        lseek(fd,0,SEEK_SET);
        if(read(fd,&x,sizeof(x))<0)
            handle_error("Eroare la read");
        sleep(1);
    }
    char s[100];
    scanf("%s",s);
    int fd2 = open("replici-parinte.txt",O_RDWR|O_CREAT,0666);
    if(fd2<0)
        handle_error("Eroare la open");
    if(write(fd2,s,strlen(s))<0)
        handle_error("Eroare la scriere");
    if(close(fd2)<0)
        handle_error("Eroare la close");
    lseek(fd,0,SEEK_SET);
    if(write(fd,&unu,sizeof(unu))<0)
        handle_error("Eroare la write");
    if(close(fd)<0)
        handle_error("Eroare la close");  
}
void dialog_fiu()
{
    int fd = open("flag.bin",O_RDWR|O_CREAT,0666);
    if(fd<0)
        handle_error("Eroare la open");
	int x = -1;
	while(x!=unu)
    {
        lseek(fd,0,SEEK_SET);
        if(read(fd,&x,sizeof(x))<0)
            handle_error("Eroare la read");
        sleep(1);
    }
    char s[100];
    scanf("%s",s);
    int fd2 = open("replici-fiu.txt",O_RDWR|O_CREAT,0666);
    if(fd2<0)
        handle_error("Eroare la open");
    if(write(fd2,s,strlen(s))<0)
        handle_error("Eroare la scriere");
    if(close(fd2)<0)
        handle_error("Eroare la close");
    lseek(fd,0,SEEK_SET);
    if(write(fd,&zero,sizeof(zero))<0)
        handle_error("Eroare la write");
    if(close(fd)<0)
        handle_error("Eroare la close");  

int main()
{
	pid_t pid_fiu;

    int fd = open("flag.bin",O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd<0)
        handle_error("Eroare la open");
    if(write(fd,&zero,sizeof(zero))<0)
        handle_error("Eroare la write");
    if(close(fd)<0)
        handle_error("Eroare la close");
	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");  return 1;
	}
	if(pid_fiu == 0)
	{ 
		printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid()); usleep(200);
		dialog_fiu();
	}
	else
	{   
		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid()); usleep(200);
		dialog_tata();
	}
	printf("\nSfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
	return 0;
}
