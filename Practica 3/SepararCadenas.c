#include <string.h>
#include <stdio.h>

int main () {
	int i = 0;
	char str[80] = "This is  www.tutorialspoint.com  website";
	const char s[2] = " ";
	char *token;
	char palabras[5][100];

	printf("Programa de separacion de cadenas\n");
	/* get the first token */
	token = strtok(str, s);

	/* walk through other tokens */
	while( token != NULL ) {
		printf( "%s \n", token );
		strcpy(palabras[i++],token);
		token = strtok(NULL, s);
	}
	printf("\n");
	for(i=0;i<4;i++)
		printf("%s ",palabras[i]);

	return(0);
}
