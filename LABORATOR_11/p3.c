/*
  Filename: p3.c

  Programul de mai jos ar trebui să ofere următoarea funcționalitate:

    Procesul principal creează un fiu.
    Procesul fiu se reacoperă cu comanda ps, executată cu opțiunea -o pid,user,args.
    În acest timp, procesul părinte așteaptă să se termine fiul, apoi afișează un mesaj.

*/

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h> //lipsea libraria
#include <stdlib.h> //lipsea libraria
int main(int argc, char* argv[])
{

	int pid3;

	pid3 = fork();

	if(pid3 == -1)
	{
		perror("failed to create  child process");
		return -1;
	}

	if(pid3 != 0) //pid3 nu pid
	{
		wait(NULL);
		printf("Procesul fiu a executat comanda ps.\n"); //printf nu print; erau si doua de punct virgula
	}
	else
	{
		char* parametru[] = {"ps","-o","pid,user,args",NULL}; //lipsesc virgulele : pid,user,args nu pid user args
		execvp("ps", parametru);
		abort();
	}

	return 0; //return 0
}

