#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct monkey{
	//items
	int * items;
	int inv;

	//operation
	char op;
	int op2;

	//test
	int div;
	int target_t;
	int target_f;

	//inspections
	int inspections;
};

void print_monkey(struct monkey * m){
	printf("Items (%d): ",m->inv);
	for(int i=0; i<m->inv; i++){
		printf(" -- %d",m->items[i]);
	}
	printf("\nOp: %c,%d",m->op,m->op2);
	printf("\nTs: %d?%d:%d",m->div,m->target_t,m->target_f);
	printf("\nIs: %d\n",m->inspections);
}

int main(){
	FILE *in = fopen("monkeyinput","r");
	//FILE *in = fopen("example","r");
	char buffer[255];
	char * id [6] = {
	"Monkey ",
	"  Starting items: ",
	"  Operation: new = old ",
	"  Test: divisible by ",
	"    If true: throw to monkey ",
	"    If false: throw to monkey "}; 

	//Read # of monkeys and # of items
	int monkey_index = 0;
	int total_items = 0;
	while(fgets(buffer,255,in) && !feof(in)){
		if(!strncmp(buffer,id[0],strlen(id[0])))
			monkey_index = buffer[strlen(id[0])]-'0';
		if(!strncmp(buffer,id[1],strlen(id[1]))){
			total_items++;
			char * cursor = &buffer[strlen(id[1])];
			while(*cursor != '\n')
				if(*(cursor++) == ',')
					total_items++;
		}
	}
	//printf("MONKEYS: %d\nITEMS: %d\n",monkey_index+1,total_items);
	rewind(in);

	//Monkey factory
	struct monkey monkeys[monkey_index+1];
	for(int i=0; i<=monkey_index; i++){
		monkeys[i].items = malloc(sizeof(int)*total_items);
		monkeys[i].inspections = 0;
	}
	while(fgets(buffer,255,in) && !feof(in)){
		//Monkey
		if(!strncmp(buffer,id[0],strlen(id[0])))
			monkey_index = buffer[strlen(id[0])]-'0';
		//Starting Items
		if(!strncmp(buffer,id[1],strlen(id[1]))){
			int current_item=0;
			char * cursor = &buffer[strlen(id[1])];
			while(*cursor !='\n'){
				int item_level = 0;
				while(!isdigit(*cursor))
					cursor++;
				while(isdigit(*cursor)){
					item_level*=10;
					item_level+=*(cursor++)-'0';
				}
				monkeys[monkey_index].items[current_item] = item_level;
				current_item++;
			}
			monkeys[monkey_index].inv = current_item;
		}
		//Operation
		if(!strncmp(buffer,id[2],strlen(id[2]))){
			if(buffer[strlen(id[2])+2]=='o'){
				monkeys[monkey_index].op = '^';
				monkeys[monkey_index].op2 = 0;
			}else{
				char * cursor = &buffer[strlen(id[2])];
				monkeys[monkey_index].op = buffer[strlen(id[2])];
				cursor+=2;
				int op2 = 0;
				while(isdigit(*cursor)){
					op2*=10;
					op2+=*(cursor++)-'0';
				}
				monkeys[monkey_index].op2 = op2;
			}
		}
		//Test Div
		if(!strncmp(buffer,id[3],strlen(id[3]))){
			char * cursor = &buffer[strlen(id[3])];
			int div = 0;
			while(isdigit(*cursor)){
				div*=10;
				div+=*(cursor++)-'0';
			}
			monkeys[monkey_index].div = div;
		}
		//Test True
		if(!strncmp(buffer,id[4],strlen(id[4])))
			monkeys[monkey_index].target_t = buffer[strlen(id[4])]-'0';
		//Test False
		if(!strncmp(buffer,id[5],strlen(id[5])))
			monkeys[monkey_index].target_f = buffer[strlen(id[5])]-'0';
	}

	/*/Print Monkeys
	for(int i=0; i<=monkey_index; i++){
		printf("Monkey %d\n",i);
		print_monkey(&(monkeys[i]));
	}
	*/

	//Rounds
	for(int i=0; i<20; i++){
		for(int i=0; i<=monkey_index; i++){
			struct monkey * m = &(monkeys[i]);
			//Inspections
			m->inspections += m->inv;
			for(int i=0; i<m->inv; i++){
				int item = m->items[i];
				switch(m->op){
					case '+':
						item+=m->op2;
						break;
					case '-':
						item-=m->op2;
						break;
					case '*':
						item*=m->op2;
						break;
					case '/':
						item/=m->op2;
						break;
					case '^':
						item*=item;
						break;
				}
				item/=3;
				struct monkey * m2 = !(item%m->div)?
					&(monkeys[m->target_t]):
					&(monkeys[m->target_f]);
				m2->items[m2->inv]=item;
				m2->inv++;
			}
			m->inv=0;
		}
	}

	/*/Print Monkeys
	for(int i=0; i<=monkey_index; i++){
		printf("Monkey %d\n",i);
		print_monkey(&(monkeys[i]));
	}
	*/

	//PART ONE
	int max1, max2;
	max1 = max2 = 0;
	for(int i=0;i<=monkey_index; i++){
		int inspections = monkeys[i].inspections;
		if(inspections>max1){
			if(max1>max2)
				max2=max1;
			max1=inspections;
		}
		else if(inspections>max2)max2=inspections;
	}
	printf("PART ONE: %d",max1*max2);

	//Free memory
	for(int i=0; i<=monkey_index; i++){
		free(monkeys[i].items);
	}
}
