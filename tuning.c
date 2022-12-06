#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool is_start(char * current){
	for(int i=4; i>0; i--)
		if(strchr(current,current[i])!=&current[i])
			return false;
	return true;
}

int main(){
	int processed = 4;

	FILE *in = fopen("tuninginput","r");
	char buffer [5000];
	fscanf(in,"%s",buffer);
	char * cursor = buffer;

	char * current = malloc(sizeof(char)*5) ;
	current[4]='\0';
	memcpy(current,cursor++,4);

	while(!is_start(current)){
		memcpy(current,cursor++,4);
		processed++;
	}
	printf("%d",processed);

	free(current);

}
