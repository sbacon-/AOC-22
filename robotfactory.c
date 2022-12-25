#include <stdio.h>
#include <ctype.h>

struct blueprint{
	int number;
	int ore_cost;
	int clay_cost;
	int obsidian_ore;
	int obsidian_clay;
	int geode_ore;
	int geode_obsidian;
};
struct res_pack{
	int ore;
	int clay;
	int obsidian;
	int geode;
};

int parse_digit(char * cursor);
int quality(struct blueprint * bp);
int simulate(struct blueprint * bp, struct res_pack * inv, struct res_pack * robots, int time);
int time_till(int index, struct blueprint * bp, struct res_pack *inv, struct res_pack *robots);

int main(){
	//FILE *in = fopen("blueprints","r");
	FILE *in = fopen("example","r");
	char buffer [255];
	//COUNT OF BLUEPRINTS
	int bp_count = 0;
	while(fgets(buffer,255,in) && !feof(in))
		bp_count++;
	rewind(in);

	struct blueprint blueprints[bp_count];
	for(int i=0; i<bp_count; i++){
		fgets(buffer,255,in);
		char * cursor = buffer;
		//BLUEPRINT #
		while(!isdigit(*(++cursor)));
		blueprints[i].number = parse_digit(cursor);
		cursor+=3;
		//ORE ROBOT COST
		while(!isdigit(*(++cursor)));
		blueprints[i].ore_cost = parse_digit(cursor);
		cursor+=3;
		//CLAY ROBOT COST
		while(!isdigit(*(++cursor)));
		blueprints[i].clay_cost = parse_digit(cursor);
		cursor+=3;
		//OBSIDIAN_ORE COST
		while(!isdigit(*(++cursor)));
		blueprints[i].obsidian_ore = parse_digit(cursor);
		cursor+=3;
		//OBSIDIAN_CLAY COST
		while(!isdigit(*(++cursor)));
		blueprints[i].obsidian_clay = parse_digit(cursor);
		cursor+=3;
		//GEODE_ORE COST
		while(!isdigit(*(++cursor)));
		blueprints[i].geode_ore = parse_digit(cursor);
		cursor+=3;
		//GEODE_OBSIDIAN COST
		while(!isdigit(*(++cursor)));
		blueprints[i].geode_obsidian = parse_digit(cursor);
	}
	for(int i=0; i<bp_count; i++)
		printf("BLUEPRINT: %d -> {%d,%d,{%d,%d},{%d,%d}}\n",blueprints[i].number,
				blueprints[i].ore_cost,
				blueprints[i].clay_cost,
				blueprints[i].obsidian_ore,
				blueprints[i].obsidian_clay,
				blueprints[i].geode_ore,
				blueprints[i].geode_obsidian);

	int total = 0;
	for(int i=0; i<bp_count; i++){
		total+= quality(&blueprints[i]);
	}	
	printf("PART 1: %d\n",total);
	int p2 = 1;
	//for(int i=0; i<3; i++){
	for(int i=0; i<2; i++){
		printf("BLUEPRINT: %d -> {%d,%d,{%d,%d},{%d,%d}}\n",blueprints[i].number,
				blueprints[i].ore_cost,
				blueprints[i].clay_cost,
				blueprints[i].obsidian_ore,
				blueprints[i].obsidian_clay,
				blueprints[i].geode_ore,
				blueprints[i].geode_obsidian);
		struct res_pack inv = {0,0,0,0};
		struct res_pack robots = {1,0,0,0};
		int time = 32;
		int max_geode = simulate(&blueprints[i],&inv,&robots,time);
		p2*=max_geode;
	}
	printf("PART 2: %d\n",p2);
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
int quality(struct blueprint * bp){
	struct res_pack inv = {0,0,0,0};
	struct res_pack robots = {1,0,0,0};
	int time = 24;
	int max_geode = simulate(bp,&inv,&robots,time);
	printf("%d\n",max_geode);
	return max_geode * bp->number;
}

int simulate(struct blueprint * bp, struct res_pack * inv, struct res_pack * robots, int t){
	
	printf("BLUEPRINT: %d -> {%d,%d,{%d,%d},{%d,%d}}\n",bp->number,
			bp->ore_cost,
			bp->clay_cost,
			bp->obsidian_ore,
			bp->obsidian_clay,
			bp->geode_ore,
			bp->geode_obsidian);
	printf("INV: {%d,%d,%d,%d}\n",
			inv->ore,
			inv->clay,
			inv->obsidian,
			inv->geode);
	printf("TIME: %d\n",t);

	//TIME TILL NEXT ROBOT
	struct res_pack time = {
		time_till(0,bp,inv,robots),
		time_till(1,bp,inv,robots),
		time_till(2,bp,inv,robots),
		time_till(3,bp,inv,robots)
	};
	/*
	printf("TT: {%d,%d,%d,%d}\n",
			time.ore,
			time.clay,
			time.obsidian,
			time.geode);
	*/
	int max_geode = 0;
	struct res_pack sim_rob = {robots->ore,robots->clay,robots->obsidian,robots->geode};
	
	if(time.ore!=-1 && time.ore<t && t){
		struct res_pack sim_inv = {
			inv->ore+(robots->ore*time.ore)-bp->ore_cost,
			inv->clay+(robots->clay*time.ore),
			inv->obsidian+(robots->obsidian*time.ore),
			inv->geode+(robots->geode*time.ore)};
		sim_rob.ore ++;
		int current = simulate(bp,&sim_inv,&sim_rob,t-time.ore);
		if(current>max_geode)max_geode = current;
		sim_rob.ore --;
	}
	if(time.clay!=-1 && time.clay<t && t){
		struct res_pack sim_inv = {
			inv->ore+(robots->ore*time.clay)-bp->clay_cost,
			inv->clay+(robots->clay*time.clay),
			inv->obsidian+(robots->obsidian*time.clay),
			inv->geode+(robots->geode*time.clay)};
		sim_rob.clay ++;
		int current = simulate(bp,&sim_inv,&sim_rob,t-time.clay);
		if(current>max_geode)max_geode = current;
		sim_rob.clay --;
	}
	if(time.obsidian!=-1 && time.obsidian<t && t){
		struct res_pack sim_inv = {
			inv->ore+(robots->ore*time.obsidian)-bp->obsidian_ore,
			inv->clay+(robots->clay*time.obsidian)-bp->obsidian_clay,
			inv->obsidian+(robots->obsidian*time.obsidian),
			inv->geode+(robots->geode*time.obsidian)};
		sim_rob.obsidian ++;
		int current = simulate(bp,&sim_inv,&sim_rob,t-time.obsidian);
		if(current>max_geode)max_geode = current;
		sim_rob.obsidian --;
	}
	if(time.geode!=-1 && time.geode<t && t){
		struct res_pack sim_inv = {
			inv->ore+(robots->ore*time.geode)-bp->geode_ore,
			inv->clay+(robots->clay*time.geode),
			inv->obsidian+(robots->obsidian*time.geode)-bp->geode_obsidian,
			inv->geode+(robots->geode*time.geode)};
		sim_rob.geode ++;
		int current = simulate(bp,&sim_inv,&sim_rob,t-time.geode);
		if(current>max_geode)max_geode = current;
		sim_rob.geode --;
	}
	return max_geode?max_geode:inv->geode+robots->geode*t;
}
int time_till(int index, struct blueprint * bp, struct res_pack *inv, struct res_pack *robots){
	int time = 1;
	struct res_pack sim_inv = {inv->ore,
					inv->clay,
					inv->obsidian,
					inv->geode};
	switch(index){
		case 0:
			//ORE ROBOT
			while(sim_inv.ore < bp->ore_cost){
				sim_inv.ore += robots->ore;
				time++;
			}
			return time;
		case 1:
			//CLAY ROBOT
			while(sim_inv.ore < bp->clay_cost){
				sim_inv.ore += robots->ore;
				time++;
			}
			return time;
		case 2:
			//OBSIDIAN ROBOT
			if(!robots->clay)
				return -1;
			while(sim_inv.clay < bp->obsidian_clay || sim_inv.ore < bp->obsidian_ore){
				sim_inv.ore += robots->ore;
				sim_inv.clay += robots->clay;
				time++;
			}
			return time;
		case 3:
			//GEODE ROBOT
			if(!robots->obsidian)
				return -1;
			while(sim_inv.obsidian < bp->geode_obsidian || sim_inv.ore < bp->geode_ore){
				sim_inv.obsidian += robots->obsidian;
				sim_inv.ore += robots->ore;
				time++;
			}
			return time;
	}
	return -1;
}
