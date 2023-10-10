#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<sys/file.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct item{
	int p_id;
	char p_name[100];
	int cost;
	int quant;
};

struct item_msg{
	struct item t[100];
	char buf[100];
	int count;
	int receipt_id;
};




int main(){

	time_t t;
	time(&t);
	

	struct sockaddr_in serv, cli;

	int sd, sz , nsd;
	
	char buf[300];
	struct item_msg mp;
	
	sd = socket(AF_UNIX,SOCK_STREAM,0);

	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr=INADDR_ANY;
	serv.sin_port=htons(5555);

	

	if(bind(sd,(struct sockaddr *)(&serv), sizeof(serv)) != 0){
		printf("Failed connecting to the socket\n");
		close(sd);
		exit(1);
	}


	listen(sd,5);


	while(1){
	
	
	sz=sizeof(cli);
	nsd = accept(sd,(struct sockaddr *)(&cli),&sz);

	
	if(!fork()){
	close(sd);

	
	struct flock lock;
	int fd;
	struct item p;
	read(nsd,&mp,sizeof(struct item_msg));
	
	switch(mp.buf[0]){
		case '1':	
			
			fd = open("products", O_RDONLY | O_EXCL);

			lock.l_type = F_RDLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = 0;
			lock.l_len = 0;
			lock.l_pid = getpid();

			fcntl(fd,F_SETLKW,&lock);
			
			printf("Message from Client: %s\n",mp.buf);
			while( read(fd,&p,sizeof(struct item)) >0){
				if(p.p_id != -1)
				write(nsd,&p,sizeof(struct item));
	}
			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLKW,&lock);
			
			close(fd);
		break;
		
		case '2':

			fd = open("products", O_RDONLY | O_EXCL);

			lock.l_type = F_RDLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = 0;
			lock.l_len = 0;
			lock.l_pid = getpid();

			fcntl(fd,F_SETLKW,&lock);
			
			printf("Message from Client: %s\n",mp.buf);
			lseek(fd,0,SEEK_SET);
			while( read(fd,&p,sizeof(struct item)) > 0){
				if(strcmp(p.p_name,mp.t[0].p_name)==0){
					if(p.quant >= mp.t[0].quant){
						
						strcpy(mp.buf,"Added Successfully");
						p.quant = mp.t[0].quant;
						mp.t[0] = p;
							
						write(nsd,&mp,sizeof(struct item_msg));
						
}					else{
						strcpy(mp.buf,"Quantity not available");
						
						write(nsd,&mp,sizeof(struct item_msg));
}
					
					break;	
					}
				}
			
			if(strcmp(p.p_name,mp.t[0].p_name)){

				strcpy(mp.buf,"Product doesn't exist");
				write(nsd,&mp,sizeof(struct item_msg));
				 
				
			}
			lock.l_type = F_UNLCK;
			fcntl(fd,F_SETLKW,&lock);
			
			close(fd);


		break;

		case '3':
			
			
			fd = open("products", O_RDWR | O_EXCL);

			lock.l_type = F_WRLCK;
			lock.l_whence = SEEK_SET;
			lock.l_start = 0;
			lock.l_len = 0;
			lock.l_pid = getpid();

			fcntl(fd,F_SETLKW,&lock);
			
			printf("Message from Client: %s\n",mp.buf);
			lseek(fd,0,SEEK_SET);

			int flag = 1;

		for(int i=0;i< mp.count;i++){
			lseek(fd,0,SEEK_SET);	
			while( read(fd,&p,sizeof(struct item)) > 0){
				if(strcmp(p.p_name,mp.t[i].p_name)==0){
					if(p.quant >= mp.t[i].quant){
						int temp;
						temp = mp.t[i].quant;
						mp.t[i] = p;
						mp.t[i].quant = temp;
						break;
						
}					else{
						flag = 0;
						char temp[100];
						
						strcpy(temp,"Quantity not available for *");
						strcat(temp,mp.t[i].p_name);
						strcat(temp,"*-");
						strcat(buf,temp);
						
					
					}		
				break;

				}

			}

			if(strcmp(p.p_name,mp.t[i].p_name)){
				flag=0;
				char temp[100];
						
				strcpy(temp,"Product *");
				strcat(temp,mp.t[i].p_name);
				strcat(temp,"* not available");
				strcat(temp,"-");
				strcat(buf,temp);
			}
			

			
		}
					
		if(flag){
				
				
		for(int i=0;i< mp.count;i++){
			lseek(fd,0,SEEK_SET);	
			while( read(fd,&p,sizeof(struct item)) > 0){
				if(strcmp(p.p_name,mp.t[i].p_name)==0){
					p.quant -= mp.t[i].quant;
					int off = lseek(fd,0,SEEK_CUR);
					lseek(fd,off - sizeof(struct item),SEEK_SET);
					write(fd,&p,sizeof(struct item));		
					
				break;

				}

			}
		}
		
		strcpy(buf,"S");
		int receipt_id = getpid()+sizeof(buf);
		char rec[20];
		snprintf( rec, 21, "%d", receipt_id );
		mp.receipt_id=receipt_id;
		strcpy(mp.buf,buf);
		write(nsd,&mp,sizeof(struct item_msg));

			int fd_log = open("market.log",O_RDWR | O_EXCL);
			char log[200];
			lseek(fd_log,0,SEEK_END);

                      	strcpy(log,"<> TIME: ");
                        strcat(log,ctime(&t));
                        strcat(log," * Successfull Transaction\n");
			strcat(log," * Receipt ID: ");
			strcat(log,rec);
			strcat(log,"\n");

                        write(fd_log,log,sizeof(log));



			close(fd_log);


		}

		else{
			char temp[100];
			
			strcpy(temp,"E");
			strcat(buf,temp);
			strcpy(mp.buf,buf);
			write(nsd,&mp,sizeof(struct item_msg));

			int fd_log = open("market.log",O_RDWR | O_EXCL);
			char log[200];
			lseek(fd_log,0,SEEK_END);

                      	strcpy(log,"<> TIME: ");
                        strcat(log,ctime(&t));
                        strcat(log," * Unsuccessfull Transaction");

                        write(fd_log,log,sizeof(log));

			close(fd_log);

		}


		lock.l_type = F_UNLCK;
		fcntl(fd,F_SETLKW,&lock);

		close(fd);

			

		break;

		default:
			printf("IDLE\n");
			
			break;
				
	}

	
	exit(0);
	}
	else
		close(nsd);
}


	close(sd);
	

	return 0;


}
