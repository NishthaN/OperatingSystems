/* Name: Nishtha Nayar
	ID: 1610110235 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_LINE 80 // The maximum length command 
int should_run =1;
int check_for_ampersand;
int quit;
int counter;
char history[100];
char *args[MAX_LINE/2 +1 ]; //Command line arguments

void get_line();
void launch(char *args);
void from_history();
void tokenizer(char *tempmemory);
int change_Directory(char* args[]);


int main(void){

	char string;
	
	FILE *fp;
	fp = fopen("Users/nishthanayar/Desktop/history.txt ","r" );
	int a=0;
    while (!feof(fp)){				//Reading from text file into array
        fgets(history[a],80,fp);
        ++a;
    }

	

	while(should_run){
		printf("nish>");
		fflush(stdout);
		get_line(args); //calling function to read commands
		launch(args); //calling function to execute commands 
		
		
 		
	}
	for(int b=0; b<sizeof(history); b++){
			//Writing to text file from array
	fwrite(history, sizeof(char), MAX_LINE, fp);
	fclose(fp);
}
}



void get_line(){

	check_for_ampersand=0;
	should_run=1;
	counter=0;
	int i;
	int buffer_size= sizeof(char)*MAX_LINE;

	char *tempmemory = malloc(buffer_size); //allocating memory to buffer 


	if(tempmemory == NULL){ 						//when memory is not allocated to buffer
		printf("Error allocating memory");
		return;
	}

	fgets(tempmemory,buffer_size,stdin);
	history[i]= *tempmemory;
	i=i+1;

	tokenizer(tempmemory);

	

		/*int j=0;								//check code running
		for(j=0; j<=i; j++)
			printf("%c\n", args[j]);
		printf("%d\n", check_for_ampersand );
		printf("%d\n", quit); */


}

void tokenizer(char *tempmemory){

	char *token = strtok(tempmemory, " ");
	char *string;
	int i;

		while (token!= NULL){
			if((strcmp(token,"&")) == 0)
				check_for_ampersand=1;			//check for ampersand
			if((strcmp(token,"quit"))==0)					//check for terminating condition
				should_run=0;
			i=0;
			args[i]=*token;
			i+=1;
			counter+=1;
			if (strcmp(args[i],'!!'))
			{	
				string = history[sizeof(history)-1];
				tokenizer(string);
				return;
			}
			else if(strcmp(args[i], '!')){
				token =strtok(NULL, " ");
				int n= atoi(*token);
				string = history[n-1];
				tokenizer(string);
				return;
				
			}
			token = strtok(NULL, " ");
		}

}

void launch(char *args){

	pid_t pid;
	int i=0;

	pid= fork();

	if(pid ==-1 ){
		printf("Error: Forking failed\n");
		return;
	}
	if(pid==0){
		if(execvp(args[0], args) <0){
			printf("Error: Execution failed\n");
			exit(0);
		}

	
	else{
		if(strcmp(args[0],NULL))
			return;
		for(i=0; i<counter; i++ ){
			if(!strcmp(args[i], "history"))
				{from_history();
					return;
				}
			
			else if(!strcmp(args[i], "cd")){
				change_Directory(args);
				return;
			}
			execvp(args[i], args);
			if(check_for_ampersand==1)
				wait(&pid);
		}
		
	}
}
	return;


}

void from_history(){


	int i=0;
	
	for(i=0; i< 10; i++)      //Displaying last 10 commands

		printf("%d %c\n", i+1, history[i]);
}



int change_Directory(char* args[]){
														// If we write no path go to the home directory
	if (args[1] == NULL) {
		chdir(getenv("HOME")); 
		return 1;
	}
	
	else{ 
		if (chdir(args[1]) == -1) {
			printf(" %s: no such directory\n", args[1]);
            return -1;
		}
	}
	return 0;
}