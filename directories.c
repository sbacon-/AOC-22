#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SUBS 100
#define TARGET_SIZE 100000
#define TOTAL_DISK 70000000
#define UPDATE_REQ 30000000

struct directory{
	char * name;
	long size;
	struct directory * parent;
	struct directory ** sub_dirs;
	int subs;
};

struct directory* initialize_dir(char * name,struct directory* parent){
	struct directory *dir = malloc(sizeof(struct directory));
	dir->name = malloc(sizeof(char)*strlen(name)+1);
	strcpy(dir->name,name);
	dir->sub_dirs = malloc(sizeof(struct directory *) * MAX_SUBS);
	dir->parent = malloc(sizeof(struct directory *));
	if(parent!=NULL){
		dir->parent = parent;
		parent->sub_dirs[parent->subs]=dir;
		parent->subs++;
	}
	return dir;
}

void relay_size(struct directory* dir,long size){
	dir->size+=size;
	while(dir->parent != NULL){
		dir = dir->parent;
		dir->size+=size;
	}
}

void sum_all(struct directory* dir,long * sum){//PART ONE
	for(int i=0; i<dir->subs; i++)
		sum_all(dir->sub_dirs[i],sum);
	if(dir->size<=TARGET_SIZE)
		*sum += dir->size;
}

long search(struct directory* dir,long reqspace,long min){
	for(int i=0; i<dir->subs; i++){
		long res = search(dir->sub_dirs[i],reqspace,min);
		if(res>=reqspace && res<min)
			min = res;
	}
	if(dir->size>=reqspace && dir->size<min)
		return dir->size;
	return min;
}

void free_all(struct directory* dir){
	for(int i=0; i<dir->subs; i++)
		free_all(dir->sub_dirs[i]);
	free(dir);
}

int main(){
	FILE *in = fopen("consoleinput","r");
	char buffer [255];
	fgets(buffer,100,in);
	struct directory * current = initialize_dir("/",NULL);
	struct directory * root = current;
	for(;;){//PARSE CONSOLE INPUT
		fgets(buffer,100,in);
		if(feof(in))break;
		*(strchr(buffer,'\n'))='\0';
		int i=0;
		if(buffer[i] == '$'){
			//HANDLE COMMANDS
			if(!strcmp(buffer,"$ ls"))
				continue;
			if(!strcmp(buffer,"$ cd .."))
				current = current->parent;
			else{//cd {dir}
				for(int j=0; j<current->subs; j++){
					if(!strcmp(&buffer[5],current->sub_dirs[j]->name)){
						current = &(*current->sub_dirs[j]);
						break;
					}
				}
			}
		}
		else if(isdigit(buffer[i])){
			//FILES
			long size = 0;
			while(isdigit(buffer[i])){
				size*=10;
				size+=buffer[i++]-'0';
			}
			relay_size(current,size);
		}
		else{
			//INITIALIZE NEW DIRECTORY
			char * name = strchr(buffer,' ');
			initialize_dir(++name,current);
		}
	}

	//PART ONE
	long * sum = malloc(sizeof(long));
	*sum = 0;
	sum_all(root,sum);
	printf("PART ONE: %ld\n",*sum);
	free(sum);

	//PART TWO
	long freespace = TOTAL_DISK - root->size;
	long reqspace = UPDATE_REQ - freespace;
	long result = search(root,reqspace,UPDATE_REQ);
	printf("PART TWO: %ld\n",result);

	free_all(root);
}
