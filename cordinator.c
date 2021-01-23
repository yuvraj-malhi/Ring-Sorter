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
#include<sys/socket.h>

#include "header.h"


int NODE_SOCK, SOCK_TMP, SOCK_R, SOCK_L;


void end(int sig)
{
	close(NODE_SOCK);
	close(SOCK_TMP);
	close(SOCK_L);
	close(SOCK_R);
	printf("COMMAND: Process terminated\n");

}


int fill (int ** data)
{
	*data = (int *)malloc(sizeof(int)*MAX_DATA);

	printf("\n------------------------- DATA READ -------------------------\n\n");
	printf("DATA: Reading data from:   file <-> %s\n",DATA_FILE);

	FILE* DATA_F = fopen (DATA_FILE, "r");
	if(DATA_F == NULL)
		perror("DATA: Error opening");

	int i = 0, size = 0;
	
	fscanf (DATA_F, "%d", (*data)+size);    
	while (!feof (DATA_F))
	{  
	      size++;
	      fscanf (DATA_F, "%d", (*data)+size);      
	}
	fclose ( DATA_F );        
	
	printf("DATA: File opened\nDATA: Data is as follows:\n");

	for(int i=0; i<size; i++)
		printf("\t\t\t\t%d\n",(*data)[i]);


	return size;

}


void main(int argc, char ** argv)
{

	signal(SIGINT, end);


	int *data;
	int DSIZE;
	char temp[20];
	DSIZE = fill(&data);

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

	if(argc!=2)
	{
		printf("COMMAND: Initialize error: Please enter the number of nodes are CLA\n");
		exit(0);
	}

	int N;
	N = atoi(argv[1]);
	printf("COMMAND: Number of node detected: %d\n", N);
	
	if( N != DSIZE )
	{
		printf("COMMAND: Error: Size of data does not match number of nodes given as CLA\n");
		exit(0);
	}
	
	
	printf("CONNECT: Connecting to node.c at:   port <-> %d\n", NODE_PORT);

	struct sockaddr_in conn;
	
	NODE_SOCK = socket(AF_INET, SOCK_STREAM, 0);
	if( NODE_SOCK == -1 )
	{
		printf("CONNECT: Error creating sokcet\n");
	        exit(0);
        }

	conn.sin_port   = htons(NODE_PORT);
	conn.sin_family = AF_INET;
	if( inet_pton(AF_INET,IP, &conn.sin_addr) < 0)
	{
		printf("CONNECT: Error with IP \n");
		exit(0);
	}

	if (connect(NODE_SOCK, (struct sockaddr *)&conn, sizeof(conn)) < 0)
		perror("CONNECT"); 

	printf("CONNECT: Connection successful\n");


	write(NODE_SOCK,argv[1], sizeof(argv[1]));

	printf("COMMAND: Number of nodes sent\n");




	SOCK_R = socket(AF_INET, SOCK_STREAM, 0);

	conn.sin_port   = htons(NODE_PORT+1);
	
	conn.sin_family = AF_INET;
	if( inet_pton(AF_INET, IP , &conn.sin_addr) < 0)
		perror("IP set error");

	
	usleep(100000);
	
	if (connect(SOCK_R, (struct sockaddr *)&conn, sizeof(conn)) < 0)
		perror("Connecting");


	printf("CONNECT: Connected to NODE <-> 0\tAT\tPORT <-> %d\n", NODE_PORT+1);





	int PORT2, len2;
	struct sockaddr_in conn2, acc2;
	len2 = sizeof(acc2);

	PORT2 = NODE_PORT+N;

	if((SOCK_TMP = socket(AF_INET, SOCK_STREAM,0))==-1) perror("SOCKET");
	conn2.sin_family = AF_INET;
	inet_pton(AF_INET, IP , &conn2.sin_addr);
	conn2.sin_port = htons(PORT2);
	if(bind(SOCK_TMP, (struct sockaddr*)&conn2, sizeof(conn2))!=0) perror("BIND");
	if(listen(SOCK_TMP,5)!=0) perror("LISTEN");
	if((SOCK_L = accept(SOCK_TMP, (struct sockaddr *) &acc2, &len2))<0) perror("ACCEPT");
	printf("CONNECT: Connected to NODE <-> %d\tAT\tPORT <-> %d\n",N-1, PORT2);





	printf("\n--------------------------- SORTING --------------------------\n\n");

	Message mes, mes2;
	mes.sndr_id = 0;
	int num_sent = 0, left_index = DSIZE-1;

	for(int i = DSIZE/2; i > 0; i=i/2){
		mes.rcvr_id = i;
		mes.num = i;
		++num_sent;
		
		for(int j = 0; j<i; ++j)
			mes.data[j] = data[left_index - i + 1 + j];
		left_index = i-1;
		
		write(SOCK_R, &mes, sizeof(mes));
		printsent(0, mes.rcvr_id,IP);
	}
	
	for(int i = 0; i<num_sent; ++i){
		read(SOCK_L, &mes2, sizeof(mes2));
		printrcvd(0, mes2.sndr_id,IP);
		if(mes2.rcvr_id != 0){
			write(SOCK_R, &mes2, sizeof(mes2));
			printsent(0, mes2.rcvr_id,IP);
			--i;
		}
		
		else{
			int z = mes2.num;
			left_index = 2*z-1;
			for(int j = 0; j<z; ++j)
				data[left_index - z + 1 + j] = mes2.data[j];		
		}
	}
	
	custom_merge(data, DSIZE);
	printf("COMMAND: Output is as follows: \n");
	
	for(int i = 0; i<DSIZE; ++i)
		printf("\t\t\t\t%d\n", data[i]);
		
	printf("\n");
	
	

	FILE* DATA_F = fopen (OUTPUT_FILE, "w");
	if(DATA_F == NULL)
		perror("DATA: Error opening");

	for(int i=0; i<DSIZE; ++i)
		fprintf(DATA_F, "%d\n", data[i]);

	fclose (DATA_F);

	printf("COMMAND: Process completed\n");
	
	end(1);
	close(NODE_SOCK);
	return;
}
