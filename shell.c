#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
	char *line, **arguments;
	printf("WELCOME TO MY SHELL!\n");
	while(1)
	{
		printf(">");
		line = readInput();
		arguments = parseInput(line):
		executeCommand();
	}
}

char* readInput()
{
	char* result = 0;
	getline(result, 0, stdin);
	return result;
}

char **parseInput(char* input)
{
	char **result;

}
