#include <stdio.h>
int main(){
	FILE *in = fopen("guide","r");
	FILE *out = fopen("out","w+");
	char buff [4];
	int score = 0;
	int result, opponent;
	int play[3];

	while(!feof(in)){
		fscanf(in,"%s",buff);
		opponent = buff[0];
		if(opponent>'C')break;
		fscanf(in,"%s",buff);
		result = buff[0];
		score+=(result-'X')*3;
		switch(result){
			case 'X':
				play[0]=3;
				play[1]=1;
				play[2]=2;
				break;
			case 'Y':
				play[0]=1;
				play[1]=2;
				play[2]=3;
				break;
			case 'Z':
				play[0]=2;
				play[1]=3;
				play[2]=1;
				break;
		}
		score+=play[opponent-'A'];
		fprintf(out,"%c - %c : %d\n",result,opponent,score);
		
	}
	fclose(in);
	fclose(out);
	printf("SCORE: %d",score);
}
