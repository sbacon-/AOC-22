#include <stdio.h>
#include <ctype.h>
#include <string.h> 
#include <limits.h>

#define TARGET 2000000 
//#define TARGET 10

struct point{ int x; int y; };
int get_value(char *c);
int calculate_dist(const struct point * s,const struct point *b);
char* advance(char *c);

int main(){
	FILE *in = fopen("sensorinput","r");
	//FILE *in = fopen("example","r");
	char buffer [255];

	//GET MAX & MIN VALUES
	struct point min, max;
	max.x = max.y = INT_MIN;
	min.x = min.y = INT_MAX;
	while(fgets(buffer,255,in) && !feof(in)){
		struct point s,b;
		if(strchr(buffer,'\n')!=NULL)
			*(strchr(buffer,'\n'))='\0';
		char *cursor = buffer;
		cursor = advance(cursor);
		s.x = get_value(cursor++);
		cursor = advance(cursor);
		s.y = get_value(cursor++);
		cursor = advance(cursor);
		b.x = get_value(cursor++);
		cursor = advance(cursor);
		b.y = get_value(cursor++);
		if(b.x>max.x||s.x>max.x)max.x = b.x>s.x?b.x:s.x;
		if(b.x<min.x||s.x<min.x)min.x = b.x<s.x?b.x:s.x;
		if(b.y>max.y||s.y>max.y)max.y = b.y>s.y?b.y:s.y;
		if(b.y<min.y||s.y<min.y)min.y = b.y<s.y?b.y:s.y;
		//printf("S:(%d,%d)\nB:(%d,%d)\n",s.x,s.y,b.x,b.y);
		//printf("DIST: %d\n",calculate_dist(&s,&b));
	}
	printf("MIN:(%d,%d)\nMAX:(%d,%d)\n",min.x,min.y,max.x,max.y);

	
	/*INSTANTIATE CAVE
	struct point dim;
	dim.x = max.x-min.x;
	dim.y = max.y-min.y;
	printf("Dim:(%d,%d)\n",dim.x,dim.y);
	char cave [dim.x+1][dim.y+1];
	for(int j=0; j<=dim.y; j++)
		for(int i=0; i<=dim.x; i++)
			cave[i][j]='.';
	THIS WOULD REQUIRE TOO MUCH MEMORY
	ONLY ONE LINE IS NEEDED IN P1*/
	int dim = max.x*2;
	char l2m [dim+2];
	for(int i=0; i<dim+2; i++)
		l2m[i]='.';
	l2m[dim+1]='\0';
	
	//SENSORS AND BEACONS
	int count = 0;
	rewind(in);
	while(fgets(buffer,255,in) && !feof(in)){
		//X,Y
		struct point s,b;
		if(strchr(buffer,'\n')!=NULL)
			*(strchr(buffer,'\n'))='\0';
		char *cursor = buffer;
		cursor = advance(cursor);
		s.x = get_value(cursor++)+max.x/2;
		cursor = advance(cursor);
		s.y = get_value(cursor++);
		cursor = advance(cursor);
		b.x = get_value(cursor++)+max.x/2;
		cursor = advance(cursor);
		b.y = get_value(cursor++);

		//CAVE
		if(s.y==TARGET)
			l2m[s.x-min.x]='S';
		if(b.y==TARGET)
			l2m[b.x-min.x]='B';

		//CALCULATE COVERAGE AT BISECTION
		int dist = calculate_dist(&s,&b);
		int coverage = dist*2+1; 
		if(s.y+dist>=TARGET && s.y-dist<=TARGET){
			struct point bisect;
			bisect.x = s.x;
			bisect.y = s.y;
			while(bisect.y!=TARGET){
				bisect.y += TARGET>bisect.y?1:-1;
				coverage-=2;
			}
			//UPDATE CAVE
			bisect.x-=coverage/2;
			while(coverage != 0){
				if(l2m[bisect.x-min.x]=='.'){
					l2m[bisect.x-min.x] = '#';
					count++;
				}
				coverage--;
				bisect.x++;
					
			}

			
		}
	}
	printf("L2M: %s\n -> %d/%d",l2m,count,strlen(l2m));
}

int get_value(char *c){
	int current = 0;
	int mod = 1;
	//INPUT CHECKING
	if(*c!='='){
		printf("Could not validate input: %c, expected '='",*c);
		return 0;
	}

	c++;
	//NEGATIVE VALUES
	if(*c=='-'){
		mod*=-1;
		c++;
	}
	//GET BASE #
	while(isdigit(*c)){
		current*=10;
		current+=*c-'0';
		c++;
	}
	return current*mod;
}

char* advance(char *c){
	//VALIDATE INPUT FOR GET_VALUE()
	while(*c != '=')
		c++;
	return c;
}

int calculate_dist(const struct point * s,const struct point *b){
	//TAXI-CAB GEOMETRIC DISTANCE
	int dist = 0;
	struct point start, step;
	start.x = s->x;
	start.y = s->y;
	step.x = s->x<b->x?1:-1;
	step.y = s->y<b->y?1:-1;
	while(start.x!=b->x){
		start.x+=step.x;
		dist++;
	}
	while(start.y!=b->y){
		start.y+=step.y;
		dist++;
	}
	return dist;
}
