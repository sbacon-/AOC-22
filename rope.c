#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

struct point{
	int x,y;
};

bool checkDist(struct point head, struct point tail){
	if(tail.x<head.x-1 || tail.x>head.x+1)
		return true;
	if(tail.y<head.y-1 || tail.y>head.y+1)
		return true;
	return false;
}

int main(){
	FILE *in = fopen("ropeinput","r");
	//FILE *in = fopen("example","r");
	char buffer [8];

	//WIDTH & HEIGHT OF THE GRID
	int w,h,w_min,w_max,h_min,h_max;
	w = h = 0;
	w_min = h_min = INT_MAX;
	w_max = h_max = INT_MIN;
	while(fgets(buffer,8,in) && !feof(in)){
		char * cursor = buffer;
		cursor+=2;
		int moves = 0;
		while(*cursor != '\n'){
			moves*=10;
			moves+=*(cursor++)-'0';
		}
		switch(buffer[0]){
			case 'R':
				w+=moves;
				break;
			case 'L':
				w-=moves;
				break;
			case 'U':
				h+=moves;
				break;
			case 'D':
				h-=moves;
				break;
		}
		if(w<w_min)w_min = w;
		if(w>w_max)w_max = w;
		if(h<h_min)h_min = h;
		if(h>h_max)h_max = h;
	}
	
	//Init Grid
	w=w_max-w_min;
	h=h_max-h_min;
	char grid[h+1][w+1];
	for(int i=0; i<=h; i++)
		for(int j=0; j<=w; j++)
			grid[i][j] = '.';
	//Init Points
	struct point head,tail;
	head.x = abs(w_min);
	head.y = abs(h_min);
	tail.x = head.x;
	tail.y = head.y;
	
	//Caclulate rope physics
	rewind(in);
	while(fgets(buffer,8,in) && !feof(in)){
		char * cursor = buffer;
		cursor+=2;
		int moves = 0;
		while(*cursor != '\n'){
			moves*=10;
			moves+=*(cursor++)-'0';
		}
		char dir = buffer[0];
		while(moves!=0){
			//move head
			if(dir=='R')head.x++;
			if(dir=='L')head.x--;
			if(dir=='U')head.y++;
			if(dir=='D')head.y--;
			if(checkDist(head,tail)){
				//move tail to catch up w/ head
				tail.x = head.x;
				tail.y = head.y;
				if(dir=='R')tail.x--;
				if(dir=='L')tail.x++;
				if(dir=='U')tail.y--;
				if(dir=='D')tail.y++;
			}
			grid[tail.y][tail.x]='#';
			moves--;
		}
	}

	int vis = 0;
	for(int i=0; i<=h; i++)
		for(int j=0; j<=w; j++)
			if(grid[i][j]=='#') vis++;

	printf("PART ONE: %d",vis);
	fclose(in);
}
