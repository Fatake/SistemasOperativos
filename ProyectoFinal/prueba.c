#include <stdio.h>

int main(int argc, char **argv){
	printf("give me a bottle of rum!\n");
	//const char base_string[] = "";
	char out_string [255];
	int number = 123;
	sprintf(out_string, "%d", number);
	printf("out_string = %s \n", out_string);
	return 0;
}
