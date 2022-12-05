#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct stack{
	char nodes[8*9];
	int size;
};
void push(struct stack* s, char c){
	s->nodes[s->size]=c;
	s->size++;
}
char pop(struct stack* s){
	s->size--;
	char res = s->nodes[s->size];
	s->nodes[s->size]='\0';
	return res;
}
void reverse(struct stack* s){
	int size = s->size;
	char nodes [size];
	for(int i=size-1; i>=0; i--)
		nodes[i] = pop(s);
	for(int i=0; i<size; i++)
		push(s,nodes[size-1-i]);
}
void cratemover9001(int move, struct stack* from, struct stack* to);

int main(){
	FILE *in = fopen("crateinstructions","r");
	char buffer [255];
	struct stack* floor [9];
	for(int i=0; i<9; i++)
		floor[i]=malloc(sizeof(struct stack));

	//Populate stacks
	for(int i=0; i<8; i++){
		fgets(buffer,100,in);
		for(int i=0; i<9; i++)
			if(buffer[4*i+1]!=' ')
				push(floor[i],buffer[4*i+1]);
	}
	for(int i=0; i<9; i++)
		reverse(floor[i]);

	//Parse instructions
	fgets(buffer,100,in);
	fgets(buffer,100,in);
	for(int i=0;i<502;i++){
		fgets(buffer,100,in);
		char * cursor = buffer;
		int move=0, from, to;
		cursor+=5;//To first digit
		move+=*(cursor++)-'0';
		if(isdigit(*cursor)){
			move*=10;
			move+=*(cursor++)-'0';
		}
		cursor+=6;//To from digit
		from=(*cursor-'0')-1;
		cursor+=5;
		to=(*cursor-'0')-1;
		//MOVE THE CRATES!
		cratemover9001(move,floor[from],floor[to]);
		/*
		while(move){
			push(floor[to],pop(floor[from]));
			move--;
		}
		*/
	}

	//Result
	for(int i=0; i<9; i++)
		printf("%c",pop(floor[i]));

	for(int i=0; i<9; i++){
		free(floor[i]);
	}
}

void cratemover9001(int move, struct stack* from, struct stack* to){
	char claw[move];
	for(int i=move-1; i>=0; i--)
		claw[i]=pop(from);
	for(int i=0; i<move; i++)
		push(to,claw[i]);
}
