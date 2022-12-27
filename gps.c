#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

int parse_digit(char * cursor);
int main(){
	FILE *in = fopen("encrypt","r");
	//FILE *in = fopen("example","r");
	char buffer [64];
	int length=0;
	while(fgets(buffer,64,in) && !feof(in))
		length++;
	printf("LEN: %d\n",length);
	rewind(in);
	//POPULATE ARRAY
	char array [length][16];
	char mixed [length][16];
	for(int i=0; i<length; i++){
		fgets(buffer,64,in);
		int current = parse_digit(buffer);
		sprintf(array[i],"%d,%d",i,current);
		strcpy(mixed[i],array[i]);
	}
	printf("INITIAL ARRAY:\n");
	for(int j=0;j<length; j++)
		printf("%s ",mixed[j]);
	printf("\n");
	//MIX
	for(int i=0; i<length; i++){
		char * cursor = array[i];
		while(*(cursor++)!=',');
		int value = parse_digit(cursor);
		bool pos = value>=0;

		int index;
		for(int j=0; j<length; j++)
			if(!strcmp(mixed[j],array[i])){
				printf("%s ======= %s\n",mixed[j],array[i]);
				index = j;
				break;
			}
		//CIRCULAR ARRAY - DISPLACEMENT WON'T EFFECT THE ORDER
		int step = pos?1:-1;
		printf("%d SHIFT FROM INDEX: %d\n",value,index);
		for(int j=0; j!=value; j+=step){
			int target = index+step;
			if(target == -1)target+=length; //LEFT
			target%=length; //RIGHT
			strcpy(mixed[index],mixed[target]);
			index+=step;
			if(index == -1)index+=length; //LEFT
			index%=length; //RIGHT
		}
		strcpy(mixed[index],array[i]);
		/*
		for(int j=0;j<length; j++)
			printf("%s ",mixed[j]);
		printf("\n");
		*/
	}
	//GET RESULT
	int zero_index;
	for(int i=0; i<length; i++){
		char * cursor = mixed[i];
		while(*(cursor++)!=',');
		int value = parse_digit(cursor);
		if(!value){
			zero_index=i;
			break;
		}
	}
	printf("ZERO: %d\n",zero_index);
	int a,b,c;
	for(int i=1; i<=3000; i++){
		zero_index++;
		zero_index%=length;
		if(i==1000){
			char * cursor = mixed[zero_index];
			while(*(cursor++)!=',');
			int value = parse_digit(cursor);
			a = value;
		}
		if(i==2000){
			char * cursor = mixed[zero_index];
			while(*(cursor++)!=',');
			int value = parse_digit(cursor);
			b = value;
		}
		if(i==3000){
			char * cursor = mixed[zero_index];
			while(*(cursor++)!=',');
			int value = parse_digit(cursor);
			c = value;
		}
	}
	printf("(%d,%d,%d) -> %d\n",a,b,c,a+b+c);
}
int parse_digit(char * cursor){
	bool pos = true;
	if(*cursor == '-'){
		pos = !pos;
		cursor++;
	}
	int current = 0;
	while(isdigit(*cursor)){
		current *= 10;
		current += *cursor - '0';
		cursor++;
	}
	return pos? current: -current;
}
