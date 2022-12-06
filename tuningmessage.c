#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool is_message(char * current){
	for(int i=14; i>0; i--)
		if(strchr(current,current[i])!=&current[i])
			return false;
	return true;
}

int main(){
	int processed = 14;

	FILE *in = fopen("tuninginput","r");
	char buffer [5000];
	fscanf(in,"%s",buffer);
	char * cursor = buffer;

	char * current = malloc(sizeof(char)*15) ;
	current[14]='\0';
	memcpy(current,cursor++,14);

	while(!is_message(current)){
		memcpy(current,cursor++,14);
		processed++;
	}
	printf("%d",processed);

	free(current);

}
