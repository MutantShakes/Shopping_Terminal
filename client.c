// CLIENT


#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<stdbool.h>
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

	struct item cart[100];
	int items = 0;
	int total_sz = 0;

	while(1){
	struct sockaddr_in serv;

	int sd, sz , nsd;
	
	char buf[80];
	
	sd = socket(AF_UNIX,SOCK_STREAM,0);

	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr=INADDR_ANY;
	serv.sin_port=htons(5555);


	
		if(connect(sd,(void *)(&serv),sizeof(serv)) !=0){
			printf("Failed to Connect!!!\n");
			return 0;
		}
		
		char msg[300];
		struct item p;
		struct item_msg mp;
			
		int choice;
		printf(" 1. View Products\n 2. Add to Cart\n 3. View Cart\n 4. Proceed to Payment\n");

		scanf("%d",&choice);

		switch(choice){
			
			case 1:
				strcpy(mp.buf,"1-View Products\n");	
				write(sd,&mp,sizeof(struct item_msg));

				printf("Processing Request...\n");

				while( read(sd,&p,sizeof(p)) > 0){
				if(p.quant > 0)
				printf("P_ID: %-10dP_NAME: %-10sP_COST: %-10dP_QUANTITY: %-10d\n",p.p_id,p.p_name,p.cost,p.quant);
		}
			break;

			case 2:

				int in_cart = -1;

				struct item_msg mp;
				printf("Enter P_NAME: ");
				scanf("%s",p.p_name);
				printf("Product Quantity: ");
				scanf("%d",&p.quant);

				for(int i=0;i< total_sz;i++){
					if(cart[i].p_id != -1 && strcmp(cart[i].p_name,p.p_name) == 0 ){
						p.quant += cart[i].quant;
						in_cart = i;
						break;
					}
				}
			
				strcpy(mp.buf,"2-Add to Cart\n");
				mp.t[0] = p;
				write(sd,&mp,sizeof(struct item_msg));
			
				printf("Processing Request...\n");

				read(sd,&mp,sizeof(struct item_msg));

				printf("STATUS: %s\n",mp.buf);

				if(mp.buf[0] == 'A'){

				int free_sp = 0;
				for(int i=0;i< total_sz;i++){
					if(in_cart != -1){
						cart[in_cart]=mp.t[0];
						free_sp=1;
						break;
					}
					
					if(cart[i].p_id == -1){
						cart[i]=mp.t[0];
						items++;
						free_sp = 1;
						break;
					}
				}

				
				
				if(!free_sp){
					cart[items] = mp.t[0];
					items++;
					total_sz++;
				}

				}


				

			break;

			case 3:
				
				
			if(items > 0){
				while(1){

				if (items <= 0){
					printf("-- EMPTY CART --\n");
					break;
				}


				for(int i=0;i< total_sz;i++){
					if(cart[i].p_id != -1)
					printf("%d) P_NAME: %-10sP_QUANTITY: %-10d\n",i+1,cart[i].p_name,cart[i].quant);
				}


				printf("Edit the cart (y or n): ");
				char b;
				while(getchar() != '\n');
				scanf("%c",&b);

				if(b == 'y'){
					printf("Delete Item ID: ");
					int item;
					scanf("%d",&item);
					item--;
				
					if(item <= total_sz - 1 && cart[item].p_id != -1){ 	
					cart[item].p_id = -1;
					items--;
					printf("Item (%d) DELETED successfully\n",item+1);
					}
					else
						printf("Item (%d) doesn't exist in CART\n",item+1);


				}

				
				else break;
				}

			}			
			else 
				printf("-- EMPTY CART --\n");
					

			break;

			case 4:

			if( items > 0){

				int price = 0;
				struct item_msg mp;
				
				int j = 0;	
				for(int i = 0 ;i< total_sz;i++){
					
					if( cart[i].p_id != -1){
						
						mp.t[j] = cart[i];	
						j++;
						
						printf("%d) P_NAME: %-10sP_QUANTITY: %-10d\n",i+1,cart[i].p_name,cart[i].quant);
					}

				}
				
					

				printf("Hit ENTER to BUY products (Price of products may have changed)\n");

				while( getchar() != '\n');
				getchar();
				
				strcpy(mp.buf,"3-Buy Products\n");
				mp.count = items;

				
				write(sd,&mp,sizeof(struct item_msg));

				printf("Processing Request...\n");

				read(sd,&mp,sizeof(struct item_msg));

				strcpy(msg,mp.buf);

				
				int c = 0;
				printf(" -- ");
				while( msg[c] > 0){
					
					if(msg[c] == 'S'){
						
						for(int i=0;i < total_sz;i++)
							cart[i].p_id = -1;
						printf("SUCCESS: Transaction Successfull --\n");
						printf(" -- Receipt ID: %d\n",mp.receipt_id);
						items = 0;

						
				for(int i = 0 ;i< mp.count;i++){
					
					price += mp.t[i].cost * mp.t[i].quant;	
				}	
				
				printf("<*> Amount Paid: %d\n",price);

				
				// Receipt Log file creation
				char buf[20];
				char rec[20];
				char amt[20];
               	 		snprintf( rec, 21, "%d", mp.receipt_id );
               	 		snprintf( amt, 21, "%d", price );
						
				strcpy(buf,"receipt_");
				strcat(buf,rec);
				strcat(buf,".log");

				// Creating file
				int rc_log = open(buf,O_RDWR | O_CREAT ,0744);
				
				char rmsg[200];
				strcpy(rmsg,"<> TIME: ");
				strcat(rmsg,ctime(&t));
				strcat(rmsg," * Receipt ID: ");
				strcat(rmsg,rec);
				strcat(rmsg,"\n * Amount Paid: ");
				strcat(rmsg,amt);
				strcat(rmsg,"\n");

				write(rc_log,rmsg,sizeof(rmsg));

				close(rc_log);
				
				

				
						break;
					}
					
					if(msg[c] == 'E'){
						printf("ERROR: Transaction Unsuccessfull --\n");
						printf("Remove NOT AVAILABLE ITEMS from CART and then continue to payment\n");
						break;
					}

					
					if(msg[c] != '-')
						printf("%c",msg[c]);
					else
						printf(" --\n -- ");
						

					

				c++;
				}

			
			}

			else
				printf("No items in CART\n");	
			
			break;	

			default:
				close(sd);
				return 0;	
		}
			

	}


	




}
