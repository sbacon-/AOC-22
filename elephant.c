#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

struct valve{
	char * name; 
	int flow;
	struct valve ** tunnel;
	int t_count;
};

int max_flow(struct valve * start, struct valve ** targets, int t_count, int moves_remaining, int flow_level, char * prev, char * flowing);
int dist_to_target(struct valve * start, struct valve * target, char * visited, int distance);

int main(){
	FILE *in = fopen("elephantinput","r");
	//FILE *in = fopen("example","r");
	char buffer [255];

	//VALVE_COUNT
	int valve_count = 0;
	int target_count = 0;
	while(fgets(buffer,255,in) && !feof(in)){
		if(strchr(buffer,'\n')!=NULL)
			*(strchr(buffer,'\n'))='\0';
		valve_count++;
		char * cursor = buffer;
		while(*(cursor++)!='=');
		if(*cursor !='0')
			target_count++;
	}
	rewind(in);

	//VALVE LIST
	struct valve valves[valve_count];
	struct valve ** targets;
	targets = malloc(sizeof(struct valve *)*target_count);
	int t_index = 0;

	//INSTANTIATE NAMES AND FLOWS
	for(int i=0; i<valve_count; i++){
		valves[i].name = malloc(sizeof(char)*3);
		fgets(buffer,255,in);
		if(strchr(buffer,'\n')!=NULL)
			*(strchr(buffer,'\n'))='\0';
		char * cursor = buffer;

		//NAME
		cursor+=strlen("Valve ");
		valves[i].name[0] = *(cursor++);
		valves[i].name[1] = *(cursor++);
		valves[i].name[2] = '\0'; 

		//VALUE
		cursor+=strlen(" has flow rate=");
		int flow = 0;
		while(isdigit(*cursor)){
			flow*=10;
			flow+=*cursor-'0';
			cursor++;
		}
		valves[i].flow = flow;
		if(flow)
			targets[t_index++] = &valves[i];
			
		//printf("%s -> %d\n",valves[i].name,valves[i].flow);
	}
	rewind(in);

	//ADD CONNECTIONS
	for(int i=0; i<valve_count; i++){
		fgets(buffer,255,in);
		if(strchr(buffer,'\n')!=NULL)
			*(strchr(buffer,'\n'))='\0';
		char * cursor = buffer;
		while(*(cursor++)!=';');
		cursor+=strlen(" tunnel");
		cursor+=*cursor=='s'?
			strlen("s lead to valves "):
			strlen(" leads to valve ");
		//PATH COUNT
		int char_count, path_count;
		char_count = path_count = 0;
		while(*cursor!='\0'){
			char_count++;
			cursor++;
		}
		cursor-=char_count;
		path_count = (char_count-2)/4 + 1;
		//printf("%d -> %d\n",char_count,path_count);
		valves[i].tunnel = malloc(sizeof(struct valve *) * path_count);
		valves[i].t_count = path_count;

		//PARSE NAMES
		for(int j=0; j< path_count; j++){
			char name [3];
			name[0] = *(cursor++);
			name[1] = *(cursor++);
			name[2] = '\0'; 
			if(*cursor!='\0')cursor+=2;
			for(int k=0; k<valve_count; k++){
				if(!strcmp(valves[k].name,name)){
					valves[i].tunnel[j]=&valves[k];
					printf("%s\n",valves[k].name);
				}
			}
		}
	}

	printf("MAX FLOW: %d",
	max_flow(
		&valves[0],
		targets,
		target_count,
		30,
		0,
		valves[0].name,
		""
	));

	//FREE MEMORY
	for(int i=0; i<valve_count; i++){
		free(valves[i].name);
		free(valves[i].tunnel);
	}
	free(targets);
}

int max_flow(struct valve * start, struct valve ** targets, int t_count,  int moves_remaining, int flow_level, char * prev, char * flowing){
	int maximum = 0;
	for(int i=0; i<t_count; i++){
		if(!strcmp(start->name,targets[i]->name))
			continue;
		int dist = dist_to_target(start,targets[i],prev,1);
		if(strstr(flowing,targets[i]->name)==NULL && 
		dist <= moves_remaining){
			char newflow [strlen(flowing)+4];
			strcpy(newflow,flowing);
			newflow[strlen(flowing)]=targets[i]->name[0];
			newflow[strlen(flowing)+1]=targets[i]->name[1];
			newflow[strlen(flowing)+2]=',';
			newflow[strlen(flowing)+3]='\0';
			printf("%s + %s ---> %s\n",flowing,start->name,newflow);
			int projected_flow = flow_level + (targets[i]->flow * (moves_remaining-dist));
			int current = max_flow(
				targets[i],
				targets,
				t_count,
				moves_remaining-dist,
				projected_flow,
				targets[i]->name,
				newflow);
			if(current>maximum)
				maximum=current;
		}
	}
	return maximum? maximum:flow_level;
}
int dist_to_target(struct valve * start, struct valve * target, char * visited,int distance){
	if(!strcmp(start->name,target->name))
		return distance;
	int shortest_path = INT_MAX;
	for(int i=0; i<start->t_count; i++){
		if(strstr(visited,start->tunnel[i]->name)==NULL){
			char newvisit[strlen(visited)+4];
			strcpy(newvisit,visited);
			newvisit[strlen(visited)]=start->name[0];
			newvisit[strlen(visited)+1]=start->name[1];
			newvisit[strlen(visited)+2]=',';
			newvisit[strlen(visited)+3]='\0';
			
			int current = dist_to_target(start->tunnel[i],target,newvisit,distance+1);
			if (current<shortest_path)shortest_path=current;
		}
	}
	return shortest_path;
}
