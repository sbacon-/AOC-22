#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct node{
	char value;
	bool visited,queued;
	int x , y;
};

int main(){
	//FILE *in = fopen("example","r");
	FILE *in = fopen("map","r");
	char buffer [255];
	int width = 0, height = 0;
	while(fgets(buffer,255,in) && !feof(in)){
		*(strchr(buffer,'\n'))='\0';
		width = strlen(buffer);
		height++;
	}
	rewind(in);
	struct node map [height][width];
	struct node * start;
	struct node * end;
	width = 0, height = 0;
	while(fgets(buffer,255,in) && !feof(in)){
		*(strchr(buffer,'\n'))='\0';
		while(buffer[width]!='\0'){
			if(buffer[width]=='S')start = &map[height][width];
			if(buffer[width]=='E')end = &map[height][width];
			map[height][width].x = width;
			map[height][width].y = height;
			map[height][width].visited = false;
			map[height][width].queued = false;
			map[height][width].value = buffer[width];
			width++;
		}
		width=0;
		height++;
	}
	width = strlen(buffer);
	printf("W:%d H:%d\n",width,height);

	int steps = 0;
	start->visited = true;
	struct node * search [width*height];
	struct node * new_search [width*height];
	search[0] = start;
	start->value = 'a';
	end->value = 'z';
	int search_size = 1;
	while(!end->visited){
		int new_size = 0;
		for(int i=0; i<search_size; i++){
			int x = search[i]->x;
			int y = search[i]->y;
			struct node * current;
			if(y<height-1){
				current = &map[y+1][x];
				if(!current -> queued && !current->visited && (current->value-1 <= search[i]->value)){
					current->queued = true;
					new_search[new_size++]=current;
					printf("UP\n");
				}
			}
			if(y>0){
				current = &map[y-1][x];
				if(!current -> queued && !current->visited && (current->value-1 <= search[i]->value)){
					current->queued = true;
					new_search[new_size++]=current;
					printf("DOWN\n");
				}
			}
			if(x<width-1){
				current = &map[y][x+1];
				if(!current -> queued && !current->visited && (current->value-1 <= search[i]->value)){
					current->queued = true;
					new_search[new_size++]=current;
					printf("LEFT\n");
				}
			}
			if(x>0){
				current = &map[y][x-1];
				if(!current -> queued && !current->visited && (current->value-1 <= search[i]->value)){
					current->queued = true;
					new_search[new_size++]=current;
					printf("RIGHT\n");
				}
			}
			printf("{%d,%d} -> %c\n",search[i]->x, search[i]->y, search[i]->value);	
			search[i]->visited = true;
		}
		for(int i=0; i<new_size; i++)
			search[i] = new_search[i];
		search_size = new_size;
		steps++;
	}
	steps--;
	printf("STEPS: %d",steps);
}
