#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int, char**);
char* readInput();
char** parseInput(char*);
char* tokenize(char**);
int isDelimiter(char);
void printPrompt();
void executeCommand(char**);
void cd(char**);
void runBuiltin(char**);

static const char *BUILTINS[] = {"cd", "exit", "pwd"};

int main(int argc, char ** argv)
{
	char *line, **arguments;
	printf("WELCOME TO MY SHELL!\n");
	while(1)
	{
		printPrompt();
		line = readInput();
		arguments = parseInput(line);
		executeCommand(arguments);
	}
}

char* readInput()
{
	char* result = 0;
	ssize_t bufsize = 0;
	getline(&result, &bufsize, stdin);
	return result;
}

#define PARS_BUF_INCR 10
#define STR_BUF_INCR 20
char **parseInput(char* input)
{
	char **result;
	int spots=0;
	int i=0, outer=0, inner=0, pars_buf=10;
	if((result = malloc(pars_buf*sizeof(char*))) == NULL)
	{
		perror("mySH: malloc() ");
		exit(EXIT_FAILURE);
	}
	while(*input!='\0')
	{
		*(result+spots) = tokenize(&input);
		outer++;
		spots++;
		input++;
		//HANDLES exceeding the number of strings allocated
		if(outer>=pars_buf && (( result = realloc(result, sizeof(char*)*(pars_buf+=PARS_BUF_INCR) )) == NULL))
		{
			perror("mySH: realloc() ");
			exit(EXIT_FAILURE);
		}
	}
	*(result+spots) = NULL;
	return result;
}

/*Get the first token or word from a pointer to a string, and increments the value of said pointer to point to the delimiter following said token*/
char* tokenize(char** input)
{
	char *result, *ptr=*input;
	int size=0, str_size=STR_BUF_INCR;
	if((result = malloc(str_size*sizeof(char)))==NULL)
	{
		perror("mySH: malloc() ");
		exit(EXIT_FAILURE);
	}
	do
	{
		*(result+size*sizeof(char)) = *(*input);
		size++;
		if(size>=str_size)
		{
			if( (result = realloc(result, sizeof(char)*(str_size+=STR_BUF_INCR) ))==NULL )
			{
				perror("mySH: realloc() ");
				exit(EXIT_FAILURE);
			}
		}
	} while(!isDelimiter((*(*input)++)));
	*(*input)--;
	*(result+(size-1)*sizeof(char)) = 0;
	return result;
}

int isDelimiter(char input)
{
	int result = 0;
	switch(input)
	{
		case ' ':
		case '\0':
		case '\n':
		case '\a':
		case '\t':
		case '\r':
			result = 1;
	}
	return result;
}

void printPrompt()
{
	printf("%s ",get_current_dir_name());

	if(getuid()==0)
		printf("#");
	else
		printf("$");
}

void executeCommand(char** args)
{
	char ** tmp = args;
	int i=0, *status;
	pid_t pid;
	if(isBuiltin(args[0]))
	{
		runBuiltin(args);
	} else
	{
		pid = fork();
		if(pid==0)
		{
			if(execvp(args[0],args) ==-1)
				perror("mySH exec failed");
		} else
		{
			waitpid(pid, status, WUNTRACED);
		}
	}
}

int isBuiltin(char* command)
{
	int i, result=0;
	for(i=0; BUILTINS[i]!=NULL;i++)
	{
		if(!strcmp(command,BUILTINS[i]))
			result = 1;
	}
	return result;
}

void runBuiltin(char** args)
{
	if(!strcmp(args[0],"cd"))
		cd(args);
	if(!strcmp(args[0],"exit"))
		exit(0);
	if(!strcmp(args[0],"pwd"))
		printf("%s\n",get_current_dir_name());
}

void cd(char** args)
{
	char * destination;
	if(args[1]==NULL)
		chdir(getenv("HOME"));
	else if(args[1][0]=='~')
	{
		chdir(getenv("HOME"));
		if(args[1][1]=='/')
			chdir(&args[1][2]);
	} else
		chdir(args[1]);
}
