#include <stdio.h>
#include <ctype.h>

void render(int signal, int cycle){
	//PART TWO
	if(!((cycle)%40))
		printf("\n");
	cycle%=40;
	if(signal-1 == cycle || signal==cycle || signal+1 == cycle)
		printf("#");
	else
		printf(".");
}

int main(){
	//FILE *in = fopen("example","r");
	FILE *in = fopen("crtinput","r");
	char buffer [10];
	int cyc [] = {20,60,100,140,180,220};
	int sums [] = {0,0,0,0,0,0};
	int cycle = 1;
	int signal = 1;
	printf("#");
	while(fgets(buffer,10,in) && !feof(in)){
		int current = 0;
		if(buffer[0]=='n')
			render(signal,cycle++);
		if(buffer[0]=='a'){
			char * cursor = buffer;
			cursor+=buffer[5]=='-'?6:5;
			render(signal,cycle++);
			while(isdigit(*cursor)){
				current*=10;
				current+=*(cursor++)-'0';
			}
			if(buffer[5]=='-')
				current*=-1;
			signal+=current;
			render(signal,cycle++);
		}
		for(int i=0;i<6;i++){
			if(sums[i]==0 && cyc[i]<=cycle){
				if(cycle>cyc[i])
					sums[i]-=current;
				sums[i]+=signal;
			}
		}
	}
	//PART ONE
	int result = 0;
	for(int i=0; i<6; i++){
		printf("[%d]-%d: %d\n",i,cyc[i],sums[i]);
		result += sums[i] * cyc[i];
	}

	
	printf("%d\n",result);
	fclose(in);
}
