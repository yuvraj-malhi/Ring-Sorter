#include<time.h>
#include<stdio.h>
#include<error.h>
#include<errno.h>
#include<netdb.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include "header.h"

int SERV_FD, SERV_SOCK, SOCK_TMP, SOCK_R, SOCK_L;

int nodes;
int *CHILD;
int parent;


void endc(int sig)
{
	
	close(SERV_FD);
	close(SERV_SOCK);
	close(SOCK_TMP);
	close(SOCK_R);
	close(SOCK_L);
	exit(0);

}

void confirmation(int sig)
{
	printf("\n--------------------------- SORTING --------------------------\n\n");
}

void end(int sig)
{
	if(getpid()!=parent)
		endc(1);

	for(int i=1; i<nodes; i++)
		kill(CHILD[i],SIGUSR1);

	close(SERV_FD);
	close(SERV_SOCK);
	close(SOCK_TMP);
	close(SOCK_R);
	close(SOCK_L);
	usleep(5000);
	for(int i=1; i<nodes; i++)
		kill(CHILD[i],SIGKILL);

	printf("COMMAND: Process terminated\n");
	exit(0);
}



void main(int argc, char ** argv)
{
	parent = getpid();
	signal(SIGINT ,end );
	signal(SIGUSR1,endc);
	signal(SIGUSR2,confirmation);

	printf("\n------------------------- HOST INFO --------------------------\n\n");

	char *IP, host[100];
	struct hostent *hen;
	if(gethostname(host, sizeof(host)) == -1)
	{
		perror("GETHOSTNAME");
		exit(0);
	}
	
	if((hen = gethostbyname(host))==NULL)
	{
		perror("GETHOSTINFO");
		exit(0);
	}
	IP = inet_ntoa(*((struct in_addr*) hen->h_addr_list[0]));
	printf("HOST_INFO: Name -> %s\n",host);
	printf("HOST_INFO: IP   -> %s\n",IP);



	printf("\n------------------------- INITIALIZE -------------------------\n\n");



		
	printf("CONNECT: Opening up listening port at:   port <-> %d\n", NODE_PORT);
	SERV_FD = socket(AF_INET, SOCK_STREAM, 0);

	if( SERV_FD == -1 )
	{
		printf("SERVER: Error creating socket\n");
		exit(0);
	}

	struct sockaddr_in conn;
	bzero(&conn, sizeof(conn));	
	conn.sin_family = AF_INET;
	inet_pton(AF_INET,IP, &conn.sin_addr);
    	conn.sin_port = htons(NODE_PORT);


	if ( (bind(SERV_FD, (struct sockaddr *)&conn, sizeof(conn))) != 0 ) 
	{
		perror("SERVER: Error binding"); 
		exit(0);
	}


	if ( (listen(SERV_FD, 5)) != 0 ) 
	{
		perror("SERVER: Error listening"); 
		exit(0);
	}

	struct sockaddr_in acc;
	int len = sizeof(acc);
	
	
	SERV_SOCK = accept(SERV_FD, (struct sockaddr *)&acc, &len); 
    	if (SERV_SOCK < 0) 
	{
		perror("SERVER: Error accepting"); 
		exit(0);
	}

	printf("SERVER: Connection successful\n");

	
	char temp[10];
	read(SERV_SOCK,temp,sizeof(temp));
	
	nodes = atoi(temp);
	printf("COMMAND: Recieved number of nodes:  %d\n", nodes);


	int ID, forked, PORT, sss[2];
	
	
	CHILD = (int *)malloc(sizeof(int)*nodes);
	for(int i=0; i<nodes; i++) CHILD[ID] = 1;

	
	struct sockaddr_in conn2, acc2;
	int check =0, len2 = sizeof(acc2);


	for(ID = 1, PORT = NODE_PORT+1; ID < nodes ; ID++, PORT++)
	{
		check =0;

		if ( (SOCK_TMP = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
		{ 
			perror("SOCKET"); 
			end(1); 
		}


		if ( (SOCK_R = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
		{ 
			perror("SOCKET Error"); 
			end(1); 
		}




		conn2.sin_family = AF_INET;
		inet_pton(AF_INET,IP, &conn2.sin_addr);
    		conn2.sin_port = htons(PORT);

		if ( (bind(SOCK_TMP, (struct sockaddr *)&conn2, sizeof(conn2))) != 0 )  
		{ 
			perror("BIND Error"); 
			end(1); 
		}


		if ( (listen(SOCK_TMP, 5)) != 0 ) 
		{ 
			perror("LISTEN Error"); 
			end(1); 
		}

		CHILD[ID]=fork();

		if(CHILD[ID]==0)
		{
			close(SERV_SOCK);
			close(SERV_FD);
			break;
		}

		check=1;
		usleep(5000);

	}



	if((CHILD[ID]==0)&&(check==0))
	{

		if ( (SOCK_L = accept(SOCK_TMP, (struct sockaddr *)&acc2, &len2)) < 0 ) 
		{
		       	perror("ACCEPT Error"); 
			end(1);
		}

		printf("NODE %d: Connected with NODE <-> %d\tAT\tPORT <-> %d\n",ID, ID-1, PORT);



		int N_PORT = PORT+1;
		struct sockaddr_in cc;
		cc.sin_family = AF_INET;
		cc.sin_port   = htons(N_PORT);
		inet_pton(AF_INET,IP, &cc.sin_addr);
		
		
		while ( connect(SOCK_R, (struct sockaddr *)&cc, sizeof(cc)) == -1 );
		printf("NODE %d: Connected with NODE <-> %d\tAT\tPORT <-> %d\n",ID, ID+1, N_PORT);
		

		if(ID == nodes-1)
			kill(getppid(),SIGUSR2);



/*
	STEPS IN EXECUTION OF INDIVIDUAL NODES
		1. receive your data, and bounce away other's data
		2. send packets to your children
		3. receive packets from your children and bounce away other's data
		4. custom_merge your data
		5. send data back to your parent_id
		6. enter into infinite loop for acting like a wire for ring topology
*/

		Message mes;
		int DSIZE;
		
		// 1. receive your data from parent, and bounce away other's data
		while(1){
			read(SOCK_L, &mes, sizeof(mes));
			printrcvd(ID, mes.sndr_id,IP);
			if(mes.rcvr_id == ID)
				break;
			
			write(SOCK_R, &mes, sizeof(mes));
		}

		DSIZE = mes.num;



		// 2. send packets to your children
		Message mes2;
		mes2.sndr_id = ID;
		int num_sent = 0, left_index = DSIZE-1;


		for(int i = DSIZE/2; i > 0; i=i/2){
			mes2.rcvr_id = ID+i;
			mes2.num = i;
			++num_sent;
			
			for(int j = 0; j<i; ++j)
				mes2.data[j] = mes.data[left_index - i + 1 + j];
			left_index = i-1;
			
			write(SOCK_R, &mes2, sizeof(mes2));
			printsent(ID, mes2.rcvr_id, IP);
		}
		
		
		
		
		// 3. receive packets from your children and bounce away other's data
		for(int i = 0; i<num_sent; ++i){
			read(SOCK_L, &mes2, sizeof(mes2));
			if(mes2.rcvr_id != ID){
				write(SOCK_R, &mes2, sizeof(mes2));
				--i;
			}
			
			else{
				printrcvd(ID, mes2.sndr_id,IP);
				int z = mes2.num;
				left_index = 2*z-1;
				for(int j = 0; j<z; ++j)
					mes.data[left_index - z + 1 + j] = mes2.data[j];
			}
		}
		
		
		
		
		// 4. custom_merge your data
		custom_merge(mes.data, mes.num);
		
		
		
		
		// 5. send data back to your parent_id
		mes.rcvr_id = mes.sndr_id;
		mes.sndr_id = ID;

		write(SOCK_R, &mes, sizeof(mes));
		printsent(ID, mes2.rcvr_id, IP);
		
		
		
		
		// 6. enter into infinite loop for acting like a wire
		int read_return = 1;
		while(read_return > 0){
			read_return = read(SOCK_L, &mes2, sizeof(mes2));
			
			if(mes2.rcvr_id != ID){
				write(SOCK_R, &mes2, sizeof(mes2));
			}
		}


		
		sleep(1);
		
		endc(1);
	}


	wait(NULL);
	strcpy(temp,"1");

	printf("COMMAND: Process completed\n");

	end(1);
	return;


}
