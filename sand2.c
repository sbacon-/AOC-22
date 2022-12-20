#include <stdio.h>
#include <limits.h>
#include <ctype.h> 
#include <string.h>
struct point{
	int x;
	int y;
};

int get_value(char * cursor);
int step(int x, int y);

int main(){
	FILE *in = fopen("sandinput","r");
	//FILE *in = fopen("example","r");
	char buffer [500];

	//GET GRID DIMENSIONS
	int max_x, max_y, min_x, min_y;
	max_x = max_y = INT_MIN;
	min_x = min_y = INT_MAX;
	while(fgets(buffer,500,in) && !feof(in)){
		if(strchr(buffer,'\n')!=NULL)
			*strchr(buffer,'\n')='\0';
		char *cursor = buffer;
		while(*cursor != '\0'){
			//GET MIN & MAX X
			int x = get_value(cursor);
			while(*cursor!=',')
				cursor++;
			int y = get_value(cursor);
			while(*cursor!='>' && *cursor!='\0')
				cursor++;
			if(x>max_x) max_x = x;
			if(x<min_x) min_x = x;
			if(y>max_y) max_y = y;
			if(y<min_y) min_y = y;
		}
	}
	printf("MAX (%d,%d)\nMIN (%d,%d)\n",max_x,max_y,min_x,min_y);
	//INSTANTIATE CAVE
	char cave [max_x+max_y][max_y+2];
	for(int j = 0; j<max_y+2; j++)
		for(int i = 0; i<max_x+max_y; i++)
			cave[i][j] = '.';
	for(int i = 0; i<max_x+max_y; i++){
		cave[i][max_y+2]='#';
	}
	rewind(in);
	//ROCKS
	while(fgets(buffer,500,in) && !feof(in)){
		if(strchr(buffer,'\n')!=NULL)
			*strchr(buffer,'\n')='\0';
		char *cursor = buffer;
		struct point p;
		p.x = get_value(cursor);
			while(*cursor!=',')
				cursor++;
		p.y = get_value(cursor);
			while(*cursor!='>' && *cursor!='\0')
				cursor++;
		cave[p.x][p.y]='#';
		while(*cursor != '\0'){
			struct point next; 
			next.x = get_value(cursor);
				while(*cursor!=',')
					cursor++;
			next.y = get_value(cursor);
				while(*cursor!='>' && *cursor!='\0')
					cursor++;
			cave[next.x][next.y] = '#';
			if(p.x == next.x)
				for(int i=p.y; i!=next.y; i+=step(p.y,next.y))
					cave[p.x][i] = '#';
			if(p.y == next.y)
				for(int i=p.x; i!=next.x; i+=step(p.x,next.x))
					cave[i][p.y] = '#';
			p.x = next.x;
			p.y = next.y;
		}
	}
	//PRINT ROCKS
	for(int j = min_y; j<=max_y+4; j++){
		for(int i = min_x; i<=max_x; i++)
			printf("%c",cave[i][j]);
		printf("\n");
	}

	//SAND
	struct point sand;
	sand.x = 500;
	sand.y = 0;
	int grains = 0;
	for(;;){
		//DOWN
		if(cave[sand.x][sand.y+1]=='.'){
			sand.y++;
		}
		//DOWN LEFT
		else if(cave[sand.x-1][sand.y+1]=='.'){
			sand.x--;
			sand.y++;
		}
		//DOWN RIGHT
		else if(cave[sand.x+1][sand.y+1]=='.'){
			sand.x++;
			sand.y++;
		}
		else{
			cave[sand.x][sand.y]='0';
			grains++;
			if(sand.x==500 && sand.y==0)
				break;
			sand.x = 500;
			sand.y = 0;
		}
		//PRINT GRID
		/*
		for(int j = min_y; j<=max_y; j++){
			for(int i = min_x; i<=max_x; i++)
				printf("%c",cave[i][j]);
			printf("\n");
		}
		*/
			
	}
	printf("%d\n",grains);
}

int get_value(char * cursor){
	while(!isdigit(*cursor))
		cursor++;
	int current = 0;
	while(isdigit(*cursor)){
		current*=10;
		current+=*cursor-'0';
		cursor++;
	}
	return current;
}

int step(int x, int y){
	return x>y? -1 : 1;
}
