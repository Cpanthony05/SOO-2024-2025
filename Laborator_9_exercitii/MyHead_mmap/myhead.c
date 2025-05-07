#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
void handle_error(const char *mesaj)
{
    perror(mesaj);
    exit(EXIT_FAILURE);
}

void print_head(char* s,int n, int n_lines, int n_bytes) {
    int bytes_read;
    int lines_count = 0, bytes_count = 0;
    
    for (int i = 0; i < n; i++) {
        if (n_bytes > 0 && bytes_count >= n_bytes) return;
        printf("%c",s[i]);
        bytes_count++;
        if (buffer[i] == '\n') {
            lines_count++;
            if (n_lines > 0 && lines_count >= n_lines) return;
        }
    }
    
}

int main(int argc, char *argv[])
    {
    int n_lines = 10, n_bytes = -1;
    int file_start = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n_lines = atoi(argv[++i]);
            file_start = i + 1;
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            n_bytes = atoi(argv[++i]);
            file_start = i + 1;
        }
    }

    for (int i = file_start; i < argc; i++)
    {
        int fd = open(argv[i], O_RDONLY);
        if(fd<0)
            handle_error("EROARE LA DESCHIDERE FISIER DE CITIRE");
        struct stat st;
        if(fstat(fd,&st)<0)
            handle_error("EROARE LA FSTAT PENTRU FISIER DE CITIRE");
        char* s = mmap(NULL,st.st_size,PROT_READ,0);
        if(s==MAP_FAILED)
            handle_error("EROARE LA MAPARE");
        print_head(s,st.st_size, n_lines, n_bytes);
        close(fd);
    }
    return EXIT_SUCCESS;
}