// UDP client program 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 5000 
#define MAXLINE 1024 

//fucntion used to get a file size
long int filesize(char* file){

	FILE * f = fopen(file,"rb");
	fseek(f,0, SEEK_END);
	
	long int size = ftell(f);
	fclose(f);

	return size;


}

int main(int argc, char** argv) 
{ 

	if(argc < 2)
	{
		printf( "provide the port and ip address\n");
		exit(1);
	}
	//extract the arguments from the command line.
	char* ipAddress = argv[1];
	int port = atoi( argv[2] );
	printf("%s\n", argv[1]);

	int sockfd; 
	char buffer[MAXLINE]; 
	char* message = "Hello Server"; 
	struct sockaddr_in servaddr; 

	int n, len; 
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 

	memset(&servaddr, '\0', sizeof(servaddr)); 

	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(port); 

	//convert the ip address to binary form
	servaddr.sin_addr.s_addr = inet_addr(ipAddress); 


	char fileName[20]; // buff memory to store file name;
	int slen = sizeof(servaddr);
	printf("enter file name w/extension: ");
	scanf("%s", &fileName);
	printf("sending file name...\n");
	int sentornot = sendto(sockfd, fileName, MAXLINE, 0, (struct sockaddr *) &servaddr,slen); //send filename to server.
 	printf("sent..%d\n", sentornot);
	memset(&buffer, 0, sizeof(buffer));

	long int size = filesize(fileName); //get file size
	char sizechar[10];
	printf("got file size: %d\n", size);
	sprintf(sizechar, "%d", size);
	sendto(sockfd, sizechar, MAXLINE, 0, (struct sockaddr *) &servaddr,slen); //send file size to the server

	FILE *f = fopen(fileName, "rb"); //create a file with a filename name
	
	int itr =0;
	do{
		printf("sending...\n");
		memset(&buffer,0,sizeof(buffer));
		fread(buffer, 256, 1,f);
		//puts(buffer);
		
		if(sendto(sockfd, buffer, 256, 0, (struct sockaddr *) &servaddr,slen)<0){
			printf("error at sending..\n"); exit(1);
		}
		itr++;
		printf("itr: %d size %d\n", itr, len);

	}while(itr*256<size); // run this loop to send 256 data packets
			     // untill we have no more 256 data packets to send
	
	int rem = size % 256;
	fread(buffer, rem, 1, f);
	sendto(sockfd, buffer, rem, 0, (struct sockaddr *) &servaddr,&slen);
	fclose(f);

	close(sockfd); 
	return 0; 
} 
