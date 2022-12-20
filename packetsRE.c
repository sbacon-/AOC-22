#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct packet{
	int values;
	int * int_values;
	struct packet * p_values;
	int cursor;
};

void build_packet(struct packet *p, char *buffer);
int count_values(struct packet *p, char *buffer);
void populate_values(struct packet *p, char *buffer);
void free_packet(struct packet *p);
void print_packet(struct packet *p);
bool compare(struct packet *p1, struct packet *p2, bool shallow);

int main(){
	FILE * in = fopen("packets","r");
	//FILE * in = fopen("example","r");
	char buffer1 [255];
	char buffer2 [255];
	char buffer3 [2];

	int sum, index;
	sum = index = 0;

	while(fgets(buffer1,255,in) && fgets(buffer2,255,in) && !feof(in)){
		index++;
		struct packet p1, p2;
		build_packet(&p1,buffer1);
		build_packet(&p2,buffer2);
		printf("COMPARE%d\n",index);
		if(p1.values==0 && p2.values==0){
			if(strcmp(buffer1,buffer2)>=0){
				sum+=index;
			}
			printf("EMPTY!\n");
		}
		else if(compare(&p1,&p2,false)){
			sum+=index;	
			printf("PASS!\n");
		}else{
			printf("FAIL!\n");
		}
		
		fgets(buffer3,2,in);//BLANK LINE
	}
	printf("PART ONE: %d\n",sum);
}

void build_packet(struct packet *p, char *buffer){
	if(strchr(buffer,'\n')!=NULL)
		*(strchr(buffer,'\n'))='\0';
	printf("BUILD_PACKET: %s\n",buffer);
	p->values = count_values(p,buffer);
	p->int_values = malloc(sizeof(int)*p->values);
	p->p_values = malloc(sizeof(struct packet)*p->values);
	for(int i=0; i<p->values; i++)
		p->int_values[i] = -2;
	p->cursor = 0;
	populate_values(p,buffer);
	p->cursor = 0;
	print_packet(p);
}

int count_values(struct packet *p, char *buffer){
	int values = 0;
	char * cursor = buffer;
	while(*(cursor++)!='\0'){
		if(isdigit(*cursor)){
			values++;
			while(*cursor != ',' && *cursor !='\0')
				cursor++;
		}
	}
	printf("COUNT_VALUES: %d\n",values);
	return values;
}

void populate_values(struct packet *p, char *buffer){
	printf("POPULATE_VALUES: \n");
	char * cursor = buffer;
	cursor++;
	while(*cursor!='\0'){
		if(isdigit(*cursor)){//INTEGERS
			int current = 0;
			while(*cursor != ',' && *cursor != ']' && *cursor != '\0'){
				current*=10;
				current+=*cursor-'0';
				cursor++;
			}
			p->int_values[p->cursor++] = current;
		}
		else if(*cursor == '['){//LISTS
			char temp [255];
			strcpy(temp,cursor);
			int sub_arrays = 0;
			char * t_cursor = temp;
			while(*(++t_cursor) != ']' || sub_arrays!=0){
				if(*t_cursor == '[')
					sub_arrays++;
				if(*t_cursor == ']' && sub_arrays!=0)
					sub_arrays--;
				cursor++;
			}
			*(++t_cursor) = '\0';
			struct packet t_packet; 
			build_packet(&t_packet,temp);
			//DEBUG
			p->p_values[p->cursor] = t_packet;
			for(int i=0; i<t_packet.values; i++)
				p->int_values[p->cursor++] = -1;
		}
		cursor++;
	}
}

void free_packet(struct packet * p){
	free(p->int_values);
	free(p->p_values);
}

bool compare(struct packet *p1, struct packet *p2, bool shallow){
	while(p1->cursor<p1->values && p2->cursor<p2->values){
		int left = p1->int_values[p1->cursor];
		int right = p2->int_values[p2->cursor];
		if(left != -1 && right != -1){
			if(left>right)
				return false;
			p1->cursor++;
			p2->cursor++;
		}
		else if(left == -1 && right == -1){
			struct packet p_left = p1->p_values[p1->cursor];
			struct packet p_right = p2->p_values[p2->cursor];

			if(!compare(&p_left,&p_right,true))
				return false;

			
			p1->cursor += p_left.values;
			p2->cursor += p_right.values;
		}
		else if(left == -1){
			struct packet p_left = p1->p_values[p1->cursor];
			struct packet conv;
			char temp [255];
			sprintf(temp,"[%d]",right);
			build_packet(&conv,temp);
			if(!compare(&p_left,&conv,true))
				return false;
			p1->cursor += p_left.values;
			p2->cursor++;
		}
		else if(right == -1){
			struct packet p_right = p2->p_values[p2->cursor];
			struct packet conv;
			char temp [255];
			sprintf(temp,"[%d]",left);
			build_packet(&conv,temp);
			if(!compare(&conv,&p_right,true))
				return false;
			p1->cursor++;
			p2->cursor += p_right.values;
		}
	}
	if(p1->cursor < p1->values && p2->cursor >= p2->values && !shallow)
		return false;
	return true;
}

void print_packet(struct packet *p){
	return ;
	printf("VALUES:%d\tCURSOR:%d\n",p->values,p->cursor);
	printf("[");
	for(int i=0; i<p->values; i++){
		printf("%d",p->int_values[i]);
		if(i+1!=p->values)
			printf(", ");
	}
	printf("]\n");
}
