#include <stdio.h>

int main(){
	FILE *in = fopen("assignments","r");
	char buffer [255];
	char delim [4] = {'-',',','-','\0'};
	int pairs = 0;
	for(int i=0; i<1000; i++){
		int index = 0;
		int results [4] = {0};
		fscanf(in,"%s",buffer);
		char * cursor = buffer;
		while(index<5){
			if(*cursor==delim[index]){
				cursor++;
				index++;
			}else{
				results[index]*=10;
				results[index]+=*(cursor++)-'0';
			}
		}
		if(!(results[1]<results[2] || results[3]<results[0]))
			pairs++;
		/*
		if((results[0]<=results[2] && results[1]>=results[3]) ||
			results[2]<=results[0] && results[3]>=results[1])
			pairs++;
		else if(!(
		*/
		printf("%d-%d,%d-%d\n",results[0],results[1],results[2],results[3]);
	}
	printf("%d\n",pairs);

	fclose(in);
}
