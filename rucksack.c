#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(){
	FILE *in = fopen("rucksack","r");
	char buffer [255];
	int priority = 0;
	while(!feof(in)){
		fscanf(in,"%s",buffer);
		int len = strlen(buffer);
		printf("%d: %s \n",len,buffer);
		char one [(len/2)+1];
		char two [(len/2)+1];
		for(int i=0; i<(len/2)+1; i++){
			one[i] = buffer[i];
			two[i] = buffer[i+len/2];
		}
		one[len/2]='\0';
		two[len/2]='\0';
		char *incorrect = strpbrk(one,two);
		if(incorrect != NULL)
			priority += islower(*incorrect)?
				*incorrect-'a'+1:
				*incorrect-'A'+27;
		printf("%s <> %s\n",one,two);
			
		printf("%d\n",priority);
	}
	fclose(in);
}
