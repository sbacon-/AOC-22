#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

struct vector3{
	int x,y,z;
};

int parse_digit(char * cursor);
bool determine_exterior(struct vector3 pos, struct vector3 dim, bool (*grid)[dim.x][dim.y][dim.z], bool (*visited) [dim.x][dim.y][dim.z]);

int main(){
	FILE *in = fopen("lavascan","r");
	//FILE *in = fopen("example","r");
	char buffer [255];
	
	//MIN-MAX
	struct vector3 max;
	max.x = max.y = max.z = INT_MIN;
	while(fgets(buffer,255,in) && !feof(in)){
		char * cursor = buffer;
		int x = parse_digit(cursor);
		while(*(cursor++)!=',');
		int y = parse_digit(cursor);
		while(*(cursor++)!=',');
		int z = parse_digit(cursor);

		if(x>max.x)max.x=x;
		if(y>max.y)max.y=y;
		if(z>max.z)max.z=z;
	}
	//INSTANTIATE GRID
	struct vector3 dim = {max.x+3,max.y+3,max.z+3};
	bool grid [dim.x][dim.y][dim.z];
	for(int i=0; i<dim.x; i++)
		for(int j=0; j<dim.y; j++)
			for(int k=0; k<dim.z; k++)
				grid[i][j][k] = false;


	//POPULATE GRID
	rewind(in);
	while(fgets(buffer,255,in) && !feof(in)){
		char * cursor = buffer;
		int x = parse_digit(cursor);
		while(*(cursor++)!=',');
		int y = parse_digit(cursor);
		while(*(cursor++)!=',');
		int z = parse_digit(cursor);
		printf("%d,%d,%d\n",x,y,z);
		grid[x+1][y+1][z+1]=true;
	}

	//CALCULATE SURFACE AREA
	int surface_area = 0;
	for(int i=0; i<dim.x; i++)
		for(int j=0; j<dim.y; j++)
			for(int k=0; k<dim.z; k++){
				if(grid[i][j][k]){
					//X
					surface_area+=!grid[i+1][j][k];
					surface_area+=!grid[i-1][j][k];
					//Y
					surface_area+=!grid[i][j+1][k];
					surface_area+=!grid[i][j-1][k];
					//Z
					surface_area+=!grid[i][j][k+1];
					surface_area+=!grid[i][j][k-1];
				}
			}
	
	//FILL AIR POCKETS USIGN DEPTH SEARCH
	for(int i=0; i<dim.x; i++)
		for(int j=0; j<dim.y; j++)
			for(int k=0; k<dim.z; k++){
				if(!grid[i][j][k]){
					struct vector3 pos = {i,j,k};
					bool visited [dim.x][dim.y][dim.z];
					for(int i=0; i<dim.x; i++)
						for(int j=0; j<dim.y; j++)
							for(int k=0; k<dim.z; k++)
								visited[i][j][k] = false;
					bool ext = determine_exterior(pos,dim,&grid,&visited);
					grid[i][j][k] = !ext;
				}
			}
	
	//CALCULATE EXT SURFACE AREA
	int ext_surface_area = 0;
	for(int i=0; i<dim.x; i++)
		for(int j=0; j<dim.y; j++)
			for(int k=0; k<dim.z; k++){
				if(grid[i][j][k]){
						//X
						ext_surface_area+=!grid[i+1][j][k];
						ext_surface_area+=!grid[i-1][j][k];
						//Y
						ext_surface_area+=!grid[i][j+1][k];
						ext_surface_area+=!grid[i][j-1][k];
						//Z
						ext_surface_area+=!grid[i][j][k+1];
						ext_surface_area+=!grid[i][j][k-1];
				}
			}
	printf("MAX: %d,%d,%d\n",max.x,max.y,max.z);
	printf("SURFACE_AREA: %d\n",surface_area);
	printf("EXT_SURFACE_AREA: %d\n",ext_surface_area);
}

int parse_digit(char * cursor){
	int current = 0;
	while(isdigit(*cursor)){
		current*=10;
		current+=*cursor-'0';
		cursor++;
	}
	return current;
}

bool determine_exterior(struct vector3 pos, struct vector3 dim, bool (*grid)[dim.x][dim.y][dim.z], bool (*visited) [dim.x][dim.y][dim.z]){
	if(pos.x == 0 || pos.y == 0 || pos.z ==0 || pos.x == dim.x-1 || pos.y==dim.y-1 || pos.z == dim.z - 1)
		return true;//REACHED THE OUTSIDE
	(*visited)[pos.x][pos.y][pos.z] = true;
	//SEARCH
	struct vector3 search = {pos.x,pos.y,pos.z};
		//X
		search.x -=1;
		if(!(*visited)[search.x][search.y][search.z] && 
		!(*grid)[search.x][search.y][search.z] &&
		determine_exterior(search,dim,grid,visited))
			return true;
		search.x +=2;
		if(!(*visited)[search.x][search.y][search.z] && 
		!(*grid)[search.x][search.y][search.z] &&
		determine_exterior(search,dim,grid,visited))
			return true;
		search.x = pos.x;
		//Y
		search.y -= 1;
		if(!(*visited)[search.x][search.y][search.z] && 
		!(*grid)[search.x][search.y][search.z] &&
		determine_exterior(search,dim,grid,visited))
			return true;
		search.y +=2;
		if(!(*visited)[search.x][search.y][search.z] && 
		!(*grid)[search.x][search.y][search.z] &&
		determine_exterior(search,dim,grid,visited))
			return true;
		search.y = pos.y;
		//Z
		search.z -= 1;
		if(!(*visited)[search.x][search.y][search.z] && 
		!(*grid)[search.x][search.y][search.z] &&
		determine_exterior(search,dim,grid,visited))
			return true;
		search.z += 2;
		if(!(*visited)[search.x][search.y][search.z] && 
		!(*grid)[search.x][search.y][search.z] &&
		determine_exterior(search,dim,grid,visited))
			return true;
	return false;
}
