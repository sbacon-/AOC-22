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
int step(int a, int b){
	if(a>b)return -1;
	if(a<b)return 1;
	return 0;
}

int main(){
	//FILE *in = fopen("example","r");
	FILE *in = fopen("ropeinput","r");
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
	//Init Rope
	int rope_len = 10;
	struct point rope [rope_len];
	for(int i=0;i<rope_len;i++){
		rope[i].x = abs(w_min);
		rope[i].y = abs(h_min);
	}
	
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
		while(moves){
			if(dir=='R')rope[0].x++;
			if(dir=='L')rope[0].x--;
			if(dir=='U')rope[0].y++;
			if(dir=='D')rope[0].y--;
			for(int i=1;i<rope_len;i++){
				if(checkDist(rope[i-1],rope[i])){
					rope[i].x += step(rope[i].x,rope[i-1].x);
					rope[i].y += step(rope[i].y,rope[i-1].y);
				}
				if(i==rope_len-1)
					grid[rope[i].y][rope[i].x]='#';
			}
			moves--;
		}
	}

	int vis = 0;
	for(int i=0; i<=h; i++){
		for(int j=0; j<=w; j++){
			if(grid[i][j]=='#') vis++;
		}
	}

	printf("PART TWO: %d",vis);
	fclose(in);
}
