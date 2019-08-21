/* Name: Nishtha Nayar
 roll no: 1610110235 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define READ_END  0  
#define WRITE_END 1  

#define BUFFER_SIZE   100 //defining buffer size 

int main(int argc, char *argv[])
{
    int fd[2]; // for two ends of pipe 
    pid_t pid;  

    

    if (argc != 3) {
        printf("Arguments not enough\n");
        exit(0);
    }
    char *infilename = argv[1];
    char *outfilename = argv[2];

    if (pipe(fd) == -1) {  //error in creating pipe
    	printf("Pipe error\n");
        exit(0);
    }

    if ((pid  = fork()) < 0) {  //error in forking
        printf("Forking error\n");
        exit(0);
    }

    if (pid > 0) {  //Parent writes to pipe
        char readbuffer[BUFFER_SIZE];
        int srcfile;
        int num;

        close(fd[READ_END]); //Close unused end of pipe

        srcfile = open(infilename, O_RDONLY);  //open file for reading only

        if (srcfile < 0) {
            printf("Parent: Error opening file %s for reading.\n", infilename);
            exit(0);
        }

        while ((num = read(srcfile, readbuffer, BUFFER_SIZE)) > 0) { //reading from source file
            if (write(fd[WRITE_END], readbuffer, num) < 0) { //writing to the pipe 
                printf("Parent: Error writing to pipe.\n");
                exit(0);
            }
        }

        if (num < 0) {
            fprintf(stderr, "Parent: Error reading from %s\n", infilename);
            exit(0);
        }

        close(srcfile); //Close file when successfully read
    }
    else {  //Child will read from pipe
        char inbuf[BUFFER_SIZE]; 
        int dstfile;
        int num;

        close(fd[WRITE_END]); //Close unused end of pipe

        dstfile = open(outfilename, O_CREAT|O_WRONLY|O_TRUNC, 0644);  //opening dest file

        if (dstfile < 0) {
            printf( "Child: Can't open file %s for writing\n", outfilename);
            exit(0);
        }

        while ((num = read(fd[READ_END], inbuf, BUFFER_SIZE)) > 0) { //reading from pipe
            if (write(dstfile, inbuf, num) < 0) { 					//writing to destination file
                printf("Child: Error writing to %s\n", outfilename);
                exit(0);
            }
        }
        if (num < 0) {
            printf("Child: Error reading from pipe.\n");
            exit(0);
        }
        close(dstfile);
        close(fd[READ_END]);
    }
    return 0;
}