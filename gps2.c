#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

long long parse_digit(char * cursor);
int main(){
	//FILE *in = fopen("encrypt","r");
	FILE *in = fopen("example","r");
	char buffer [64];
	int length=0;
	long long key = 811589153; 
	while(fgets(buffer,64,in) && !feof(in))
		length++;
	printf("LEN: %d\n",length);
	rewind(in);
	//POPULATE ARRAY
	char array [length][32];
	char mixed [length][32];
	for(int i=0; i<length; i++){
		fgets(buffer,64,in);
		long long current = parse_digit(buffer);
		sprintf(array[i],"%d,%lld",i,current*key);
		strcpy(mixed[i],array[i]);
	}
	printf("INITIAL ARRAY:\n");
	for(int j=0;j<length; j++)
		printf("%s ",mixed[j]);
	printf("\n");
	//MIX
	for(int mix_number = 0; mix_number<10; mix_number++){
	for(int i=0; i<length; i++){
		char * cursor = array[i];
		while(*(cursor++)!=',');
		long long value = parse_digit(cursor);
		bool pos = value>0;

		int index;
		for(int j=0; j<length; j++)
			if(!strcmp(mixed[j],array[i])){
				index = j;
				break;
			}
		//CIRCULAR ARRAY - DISPLACEMENT WON'T EFFECT THE ORDER
		int step = pos?1:-1;
		long long net_shift = value%length;
		printf("%lld\n",net_shift);
		for(long long j=0; j!=net_shift; j+=step){
			int target = index+step;
			if(target == -1)target+=length; //LEFT
			target%=length; //RIGHT
			strcpy(mixed[index],mixed[target]);
			index+=step;
			if(index == -1)index+=length; //LEFT
			index%=length; //RIGHT
		}
		strcpy(mixed[index],array[i]);
	}
		for(int j=0;j<length; j++){
			char * cursor = mixed[j];
			while(*(cursor++)!=',');
			long long val = parse_digit(cursor);
			printf("%lld ",val);
		}
		printf("\n");
	}
	//GET RESULT
	int zero_index;
	for(int i=0; i<length; i++){
		char * cursor = mixed[i];
		while(*(cursor++)!=',');
		long long value = parse_digit(cursor);
		if(!value){
			zero_index=i;
			break;
		}
	}
	printf("ZERO: %d\n",zero_index);
	long long a,b,c;
	for(int i=1; i<=3000; i++){
		zero_index++;
		zero_index%=length;
		if(i==1000){
			char * cursor = mixed[zero_index];
			while(*(cursor++)!=',');
			long long value = parse_digit(cursor);
			a = value;
		}
		if(i==2000){
			char * cursor = mixed[zero_index];
			while(*(cursor++)!=',');
			long long value = parse_digit(cursor);
			b = value;
		}
		if(i==3000){
			char * cursor = mixed[zero_index];
			while(*(cursor++)!=',');
			long long value = parse_digit(cursor);
			c = value;
		}
	}
	printf("(%d,%d,%d) -> %d\n",a,b,c,a+b+c);
}
long long parse_digit(char * cursor){
	bool pos = true;
	if(*cursor == '-'){
		pos = !pos;
		cursor++;
	}
	long long current = 0;
	while(isdigit(*cursor)){
		current *= 10;
		current += *cursor - '0';
		cursor++;
	}
	return pos? current: -current;
}
