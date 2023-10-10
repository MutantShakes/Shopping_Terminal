#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct item{
	int p_id;
	char p_name[100];
	int cost;
	int quant;
};

int main(){



	int fd = open("products",O_RDWR | O_EXCL);

	struct item p;
	
	lseek(fd,0,SEEK_SET);
	while( read(fd,&p,sizeof(struct item)) >0){
	if(p.p_id != -1){

	printf("ID: %-10d Name: %-10s Cost: %-10d Quantity: %-10d\n",p.p_id,p.p_name,p.cost,p.quant);

	}
}
	close(fd);
}

	
