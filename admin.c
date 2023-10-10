#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/file.h>
#include<time.h>

struct item{
	int p_id;
	char p_name[100];
	int cost;
	int quant;
};



int main(){
	

	int fd_log = open("market.log", O_RDWR | O_EXCL);
	time_t t;
	time(&t);

	while(1){
	
	char log[100];
	char name[100];
	int fd,off;
	struct item p;
	struct flock lock;

	int choice;
	printf(" 1. Add item\n 2. Update item\n 3. Delete item\n");
	scanf("%d",&choice);

	switch(choice){
	
		case 1:

			struct item p1;
			int pid=0;
			
			printf("Enter Product name: ");
			scanf("%s",p.p_name);
			//printf("Enter Product ID: ");
			//scanf("%d",&p.p_id);
			printf("Enter cost: ");
			scanf("%d",&p.cost);
			printf("Enter quantity: ");
			scanf("%d",&p.quant);
			fd = open("products",O_RDWR | O_EXCL);
			
			printf("Processing request....\n");

			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = 0;
			lock.l_len = 0;
			lock.l_pid = getpid();

			fcntl(fd,F_SETLKW,&lock);
			

			while( read(fd,&p1,sizeof(struct item)) > 0){
				if(p1.p_id == -1){
					
					off = lseek(fd,0,SEEK_CUR);
					lseek(fd, off -sizeof(struct item),SEEK_SET);
					break;
				}
				else{
					pid=p1.p_id;	
				}
						
			}

			printf("Hit Enter to Add item\n");

			while(getchar() != '\n');
			getchar();
			
			p.p_id = pid+1;

			write(fd,&p,sizeof(struct item));

			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLKW,&lock);
			
			close(fd);


			lseek(fd_log,0,SEEK_END);
			
			strcpy(log,"<> TIME: ");
			strcat(log,ctime(&t));
			strcat(log," * Item -- P_NAME: ");
			strcat(log,p.p_name);
			strcat(log," -- added to the store\n");

			write(fd_log,log,sizeof(log));
			

			printf("Item added successfully\n");
			
			
			
		
		break;

		case 2:
			printf("Enter Product Name: ");
			scanf("%s",name);

			fd = open("products",O_RDWR | O_EXCL);

			
			printf("Processing request....\n");



			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = 0;
			lock.l_len = 0;
			lock.l_pid = getpid();

			fcntl(fd,F_SETLKW,&lock);


			while( read(fd,&p,sizeof(struct item)) > 0){
				if(strcmp(p.p_name,name)==0)
					break;
			}
			
			if(strcmp(p.p_name,name)) {
				printf("Product doesn't exist\n");
				lock.l_type = F_UNLCK;
				fcntl(fd,F_SETLKW,&lock);
				close(fd);
				break;
			}


			

			
			off = lseek(fd,0,SEEK_CUR);
			lseek(fd,off - sizeof(struct item),SEEK_SET);
			
			

			printf("Hit Enter to Update item\n");

			while(getchar() != '\n');
			getchar();


			
			printf("Enter NEW quantity: ");
			scanf("%d",&p.quant);
			printf("Enter NEW cost: ");
			scanf("%d",&p.cost);

			write(fd,&p,sizeof(struct item));


			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLKW,&lock);


			close(fd);

			lseek(fd_log,0,SEEK_END);
			
			strcpy(log,"<> TIME: ");
			strcat(log,ctime(&t));
			strcat(log," * Item -- P_NAME: ");
			strcat(log,p.p_name);
			strcat(log," -- updated in the store\n");

			write(fd_log,log,sizeof(log));

			printf("Item Updated successfully\n");

		break;

		case 3:
			
			printf("Enter Product name: ");
			scanf("%s",name);

			fd = open("products",O_RDWR | O_EXCL);

			
			printf("Processing request....\n");



			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = 0;
			lock.l_len = 0;
			lock.l_pid = getpid();

			fcntl(fd,F_SETLKW,&lock);


			while( read(fd,&p,sizeof(struct item)) > 0){
				if(strcmp(p.p_name,name)==0)
					break;
			}
			
			if(strcmp(p.p_name,name)) {
				printf("Product doesn't exist\n");
				close(fd);
				lock.l_type = F_UNLCK;
				fcntl(fd,F_SETLKW,&lock);
				break;
			}


			
			off = lseek(fd,0,SEEK_CUR);
			lseek(fd,off - sizeof(struct item),SEEK_SET);


			
			printf("Hit Enter to DELETE item\n");

			while(getchar() != '\n');
			getchar();


			p.p_id = -1;
			strcpy(p.p_name,"empty");	

			write(fd,&p,sizeof(struct item));


			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLKW,&lock);


			close(fd);

			lseek(fd_log,0,SEEK_END);
			
			strcpy(log,"<> TIME: ");
			strcat(log,ctime(&t));
			strcat(log," * Item -- P_NAME: ");
			strcat(log,p.p_name);
			strcat(log," -- deleted from the store\n");

			write(fd_log,log,sizeof(log));

			printf("Item Deleted successfully\n");
		
		break;
			
			 
		default:
			return 0;
	}
}


	close(fd_log);

}
				
