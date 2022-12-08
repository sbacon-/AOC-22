#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define GRID_SIZE 99

int main(){
	int trees[GRID_SIZE][GRID_SIZE];

	//PARSE INPUT
	FILE *in = fopen("trees","r");
	char buffer [255];
	for(int i=0;;i++){
		fgets(buffer,255,in);
		if(feof(in))break;
		(*strchr(buffer,'\n'))='\0';
		for(int j=0;j<strlen(buffer);j++)
			trees[i][j]=buffer[j]-'0';
	}

	int hidden = 0;
	int max_score = 0;
	for(int i=0;i<GRID_SIZE;i++){
		for(int j=0;j<GRID_SIZE;j++){
			int up,down,left,right;
			bool h_up,h_down,h_left,h_right;//hidden
			int v_up,v_down,v_left,v_right;//view
			h_up = h_down = h_left = h_right = false;
			v_up = v_down = v_left = v_right = 0;
			up = down = i;
			left = right = j;
			//VISIBLE
			while(true){
				if((up==0 && !h_up) || 
				   (left==0 && !h_left)  || 
				   (down == GRID_SIZE-1 && !h_down)  || 
				   (right == GRID_SIZE-1) && !h_right)
					break;
				if(!h_up)up--;
				if(!h_down)down++;
				if(!h_left)left--;
				if(!h_right)right++;
				
				if(trees[i][j]<=trees[up][j]) h_up = true;
				if(trees[i][j]<=trees[down][j]) h_down = true;
				if(trees[i][j]<=trees[i][left]) h_left = true;
				if(trees[i][j]<=trees[i][right]) h_right = true;
				if(h_up && h_down && h_left && h_right){
					hidden++;
					break;	
				}
			}
			//SCENIC SCORE
			while(i-v_up>0){
					v_up++;
				int current = trees[i-v_up][j];
				if(current>=trees[i][j]){
					break;	
				}
			}
			while(i+v_down<GRID_SIZE-1){
					v_down++;
				int current = trees[i+v_down][j];
				if(current>=trees[i][j]){
					break;
				}
			}
			while(j-v_left>0){
					v_left++;
				int current = trees[i][j-v_left];
				if(current>=trees[i][j]){
					break;
				}
			}
			while(j+v_right<GRID_SIZE-1){
					v_right++;
				int current = trees[i][j+v_right];
				if(current>=trees[i][j]){
					break;
				}
			}
			
			//It took me way too long to find out the task was to multiply these
			//int score = v_up + v_down + v_left + v_right;
			int score = v_up * v_down * v_left * v_right;
			if(score>max_score)
				max_score = score;
		}
	}
	printf("PART ONE: %d\n",(GRID_SIZE*GRID_SIZE) - hidden);
	printf("PART TWO: %d\n",max_score);
	fclose(in);
}
