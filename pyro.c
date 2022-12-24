#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ELEMENT 25000
#define C_WIDTH 7

int main(){
	FILE *in = fopen("pyroclastic","r");
	//FILE *in = fopen("example","r");
	char buffer [MAX_ELEMENT];
	fgets(buffer,MAX_ELEMENT,in);
	char * current_jet = buffer;

	int tower_height = 0;
	char chasm [MAX_ELEMENT][C_WIDTH];
	for(int y=0; y<MAX_ELEMENT; y++)
		for(int x=0; x<C_WIDTH; x++)
			chasm[y][x] = '.';
	
	int shape_count = 5;
	char hline [4][C_WIDTH] = {
		{"......."},
		{"......."},
		{"......."},
		{"..@@@@."}};
	char plus  [4][C_WIDTH] = {
		{"......."},
		{"...@..."},
		{"..@@@.."},
		{"...@..."}};
	char angle [4][C_WIDTH] = {
		{"......."},
		{"....@.."},
		{"....@.."},
		{"..@@@.."}};
	char vline [4][C_WIDTH] = {
		{"..@...."},
		{"..@...."},
		{"..@...."},
		{"..@...."}};
	char square[4][C_WIDTH] = {
		{"......."},
		{"......."},
		{"..@@..."},
		{"..@@..."}};

	for(int i=0; i<2022; i++){
	//for(int i=0; i<10; i++){
		//GET SHAPE ARRAY
		int shape = i % shape_count;
		printf("%d\n",shape);
		char (*shape_arr)[4][C_WIDTH];
		switch(shape){
			case 0:
				shape_arr = &hline;
				break;
			case 1:
				shape_arr = &plus;
				break;
			case 2:
				shape_arr = &angle;
				break;
			case 3:
				shape_arr = &vline;
				break;
			case 4:
				shape_arr = &square;
				break;
		}
		//INSTANTIATE TO CHASM
		int instance_start = tower_height+6;
		for(int i=0; i<4; i++)
			for(int j=0; j<7; j++)
				chasm[instance_start-i][j]=(*shape_arr)[i][j];
		bool at_rest = false;
		int fall_dist = 0;

		//MAIN LOOP
		while(!at_rest){
			int block_height = instance_start-fall_dist;
			//PUSH
			bool can_push = true;
			char dir = *current_jet;
			int step = dir=='<'?-1:1;
			printf("PUSH:%c\n",dir);
			for(int i=block_height; i>=block_height-4; i--){
				if(!can_push)
					break;	
				for(int j=(dir=='<'?0:6); j!=(dir=='<'?6:0); j-=step)
					if(chasm[i][j]=='@' && 
						(j==(dir=='<'?0:6) || 
						chasm[i][j+step] == '#')){
							printf("%d,%d!!\n",i,j);
							can_push=false;
							break;
						}
			}
			if(can_push)
				for(int i=block_height; i>=block_height-4; i--)
					for(int j=(dir=='<'?0:6); j!=(dir=='<'?7:-1); j-=step)
						if(chasm[i][j]=='@'){
							chasm[i][j+step]='@';
							chasm[i][j]='.';
						}
			current_jet++;
			if(*current_jet == '\n')
				current_jet = buffer;
			//FALL
			bool can_fall = true;
			for(int i=block_height-4; i<=block_height; i++){
				if(!can_fall)
					break;
				for(int j=0; j<7; j++)
					if(chasm[i][j]=='@' && 
						(i==0 || chasm[i-1][j] == '#')){
							can_fall=false;
							break;
						}
			}
			if(can_fall){
				for(int i=block_height-4; i<=block_height; i++)
					for(int j=0; j<7; j++)
						if(chasm[i][j]=='@'){
							chasm[i-1][j]='@';
							chasm[i][j]='.';
						}
				fall_dist++;
			}else{
				at_rest = true;
			}
		}
		//RESTING
		int block_height = instance_start-fall_dist;
		for(int i=block_height; i>=block_height-4; i--)
			for(int j=0; j<7; j++)
				if(chasm[i][j] == '@')
					chasm[i][j] = '#';
		
		//CALCULATE TOWER_HEIGHT
		for(;;){
			bool empty = true;
			for(int i=0; i<7; i++){
				if(chasm[tower_height][i]!='.'){
					empty=false;
					break;
				}
			}
			if(!empty){
				tower_height++;
			}else{
				break;
			}
		}

		for (int i=tower_height; i>=0; i--){
			for(int j=0; j<7;j++){
				printf("%c",chasm[i][j]);
			}
			printf("\n");
		}
		printf("------------TH:%d\n",tower_height);
	}
}
