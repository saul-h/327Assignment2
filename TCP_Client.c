// TCP Client program 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

#define PORT 5000 
#define BUFF_SIZE 256 

int filesize(char* file){

	FILE * f = open(file,"r");
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
	char* ipAddress = argv[0];
	int port = atoi( argv[1] );

	int sockfd; 
	char buffer[BUFF_SIZE]; 
	char* message = "Hello Server"; 
	struct sockaddr_in servaddr; 

	int n, len; 

	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&buffer, 0, sizeof(buffer));


	// Filling server information 
	servaddr.sin_family = AF_INET; //communication domain. IPv4
	servaddr.sin_port = htons(port); 
	servaddr.sin_addr.s_addr = inet_addr(ipAddress); 

	if( bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror("binding failed");
		exit(0);
	}
	printf("Connected to port %d and ip %s\n", port, ipAddress);


	memset(buffer, 0, sizeof(buffer)); 
	strcpy(buffer, "Hello Server"); 
	write(sockfd, buffer, sizeof(buffer)); 
	printf("Message from server: "); 
	read(sockfd, buffer, sizeof(buffer)); 
	puts(buffer); 
	close(sockfd); 
} 
