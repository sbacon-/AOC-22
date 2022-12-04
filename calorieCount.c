#include <stdlib.h>
#include <stdio.h>

int main(){
	FILE *in = fopen("input","r");
	char buff[255];
	int max1,max2,max3;
	int current, item;
	max1 = max2 = max3 = 0;
	current = item = 0;
	while(!feof(in)){
		fgets(buff,255,in);
		item=atoi(buff);
		if(item){
			current+=item;
		}
		else{
			if(current>max1){
				max1=current;
			}else if(current>max2){
				max2=current;
			}else if(current>max3){
				max3=current;
			}
			current=0;
		}
	}
	fclose(in);
	printf("%d\n",max1+max2+max3);
	return 0;
}
