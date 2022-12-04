#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(){
	FILE *in = fopen("rucksack","r");
	char buffer1[255];
	char buffer2[255];
	char buffer3[255];
	char buffer4[255];
	int priority = 0;
	int count = 0;
	while(count<300){
		fscanf(in,"%s",buffer1);
		fscanf(in,"%s",buffer2);
		fscanf(in,"%s",buffer3);
		char *cursor = buffer4;
		for(int i=0; i<strlen(buffer1); i++)
			for(int j=0;j<strlen(buffer2);j++)
				if(buffer1[i]==buffer2[j])
					*(cursor++)=buffer1[i];
		*cursor = '\0';
		char *similar = strpbrk(buffer3,buffer4);
		if(similar != NULL)
			priority += islower(*similar)?
				*similar-'a'+1:
				*similar-'A'+27;
	
		count+=3;
	}
	printf("%d\n",priority);
	fclose(in);
}
