#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
struct packet{
	int * values;
	int size;
	struct packet * child;
	int * c_index;
	int c_size; };

void factory(struct packet * p, char * input){
	char * cursor = input;
	cursor++;
	p->size=1;
	p->c_size=0;

	//Calculate Size
	while(*cursor!='\0'){
		if(*cursor == ',')
			p->size++;
		if(*cursor == '['){
			p->c_size++;
			while(*cursor!=']')
				cursor++;
		}
		else
			cursor++;
	}

	//Memory allocation
	p->values = malloc(sizeof(int)*p->size);
	p->child = malloc(sizeof(struct packet)*p->c_size);
	p->c_index = malloc(sizeof(int)*p->c_size);
	p->values[0] = 0;

	//Write values
	cursor=input;
	cursor++;
	int index = 0;
	int c_index = 0;
	while(index!=p->size && *cursor!='\0'){
		int current=-1;
		//Digit Values
		while(isdigit(*cursor)){
			if(current=-1) 
				current=0;
			current*=10;
			current+=*cursor-'0';
			cursor++;
		}
		if(current!=-1){
			p->values[index++]=current;
		}
		//Packet Values
		if(*cursor == '['){
			int count = 0;
			char temp [255];
			strcpy(temp,cursor);
			char *t = temp;
			p->values[index++]=-1;
			while(!count && *cursor!=']'){
				if(*cursor=='['){
					count++;
				}
				if(*cursor=='['){
					count--;
				}
				cursor++;
				t++;
			}
			*(++t)='\0';
			p->c_index[c_index]=index;
			factory(&(p->child[c_index++]),temp);
			
		}
		else
			cursor++;
	}
	printf("SIZE: %d\n",p->size);
	for(int i=0; i<p->size; i++){
		printf(" %d ",p->values[i]);
	}
	printf("\n");
}

bool compare2(int x, const struct packet *p2){
	for(int i=0; i<p2->size; i++){
		//printf("%d -> %d\n",x,p2->values[i]);
		if(x>p2->values[i])
			return false;
	}
	return true;
}
bool compare1(const struct packet *p1,int x){
	for(int i=0; i<p1->size; i++){
		//printf("%d -> %d\n",x,p1->values[i]);
		if(x<p1->values[i])
			return false;
	}
	return true;
}

bool compare(const struct packet *p1,const struct packet *p2){
	int comp = 0;
	while(comp < p1->size){
		bool p1_pack = false;
		bool p2_pack = false;
		if(comp >= p2->size){
			//printf("SIZE FALSE");
			return false;
		}
		int i=0;
		int j=0;
		for(;i<p1->c_size; i++){
			if(p1->c_index[i]-1==comp){
				p1_pack = true;
				break;
			}
		}
		for(;j<p2->c_size; j++){
			if(p2->c_index[j]-1==comp){
				p2_pack = true;
				break;
			}
		}
		if(p1_pack && p2_pack){
			//printf("DOUBLE TRUE %d-%d\n",i,j);	
			if(!compare(&(p1->child[i]),&(p2->child[j])))
				return false;
		}else if(p1_pack){
			//printf("P1 PACK TRUE %d\n",i);
			if(!compare1(&p1->child[i],p2->values[comp]))
				return false;
			if(p1->size == 1 && p1->values[0]==-1){
				return false;
			}
		}else if(p2_pack){
			//printf("P2 PACK TRUE %d\n",j);	
			if(!compare2(p1->values[comp],&p2->child[j]))
				return false;
		}else{
			//printf("DOUBLE INT",i,j);
			if (p1->values[comp] > p2->values[comp])
				return false;
		}
		comp++;
	}
	
	return true;
}

int main(){
	//FILE *in = fopen("packets","r");
	FILE *in = fopen("example","r");
	char buffer1[255];
	char buffer2[255];
	char buffer3[2];

	int sum=0;
	int index = 1;

	while(fgets(buffer1,255,in) && fgets(buffer2,255,in) && !feof(in)){
		printf("%s",buffer1);
		printf("%s",buffer2);
		*(strchr(buffer1,'\n'))='\0';
		*(strchr(buffer2,'\n'))='\0';
		fgets(buffer3,2,in);
		struct packet p1;
		struct packet p2;
		factory(&p1,buffer1);
		factory(&p2,buffer2);


		bool order = compare(&p1,&p2);
		if(order){
			sum+=index;
			printf("%d\n",index);	
		}

		index++;

		//Free Memory
		free(p1.values);
		free(p2.values);
		free(p1.child);
		free(p2.child);
	}
	printf("%d\n",sum);
}

