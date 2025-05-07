#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>


void handle_error(const char *mesaj)
{
    perror(mesaj);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    char *in = argv[1];
    char *out = argv[2];
    char c1 = argv[3][0];
    char c2 = argv[4][0];

    int *input = open(in,O_RDONLY);
    if(access(out,F_OK) == 0)
    {
        printf("Fisierul %s exista. Vreti overwrite? (y/n)",out);
        char c;
        scanf("%c",&c);
        if(c!='y' && c!='Y')
        {
            close(input);
            return 0;
        }
    }
    int *output = open(out,O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);//0600

    struct stat st;
    if(fstat(input,&st)<0)
        handle_error("EROARE LA FSTAT");
    int diminput = st.st_size;

    if(ftruncate(output,diminput)<0)
        handle_error("EROARE LA SETAREA DIMENSIUNII FISIER OUTPUT");
    
    char* I = mmap(NULL,diminput,PROT_READ,MAP_SHARED,input,0);
    if(I==MAP_FAILED)
        handle_error("EROARE LA MAPARE INPUT");
    char* O = mmap(NULL,diminput,PROT_WRITE,MAP_SHARED,output,0);
    if(O==MAP_FAILED)
        handle_error("EROARE LA MAPARE OUTPUT");
    for(int i=0;i<diminput;i++)
    {
        if(I[i]==c1)
            O[i] = c2;
        else
            O[i] = I[i];
    }
    close(input);
    close(output);
    return 0;
}