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
	char *result_ptr = *result;
	int i=0, outer=0, inner=0, pars_buf=10;
	if((result = malloc(pars_buf*sizeof(char*))) == NULL)
	{
		perror("mySH: malloc() ");
		exit(EXIT_FAILURE);
	}
	while(*input!='\0')
	{
		//HANDLES exceeding the number of strings allocated
		if(outer>=pars_buf && (( result = realloc(result, sizeof(char*)*(pars_buf+=PARS_BUF_INCR) )) == NULL))
		{
			perror("mySH: realloc() ");
			exit(EXIT_FAILURE);
		}
		printf("string %s\n", input);
		result_ptr = tokenize(&input);
		printf("TOKENIZED");
		outer++;
		input++;
	}
	return result;
}

/*Get the first token or word from a pointer to a string, and increments the value of said pointer to point to the delimiter following said token*/
char* tokenize(char** input)
{
	printf("%s", *input);
	char *result, *ptr=*input;
	int size=0, str_size=STR_BUF_INCR;
	if((result = malloc(str_size*sizeof(char)))==NULL)
	{
		perror("mySH: malloc() ");
		exit(EXIT_FAILURE);
	}
	do
	{
		printf("count\n");
		if(size>=str_size)
		{
			if( (result = realloc(result, sizeof(char)*(str_size+=STR_BUF_INCR) ))==NULL )
			{
				perror("mySH: realloc() ");
				exit(EXIT_FAILURE);
			}
		}
		*(result+size*sizeof(char)) = *(*input);
	} while(!isDelimiter(**input));
//	printf("%s", result);
	return result;
}

int isDelimiter(char input)
{
	printf("char %c\n", input);
	int result = 0;
	switch(input)
	{
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
	printf("%s",get_current_dir_name());

	if(getuid()==0)
		printf("#");
	else
		printf("$");
}

void executeCommand(char** args)
{
	char ** tmp = args;
}
