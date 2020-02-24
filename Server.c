// Server program 
#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/stat.h>
#define PORT 5000 
#define MAXLINE 1024 
#define IP_ADDRESS "127.0.0.1"

int isFileInDirectory(char *path){
	

	printf("isnide -----%s-----\n",  path);
	FILE *f = fopen(path, "rb");
	if(f==NULL) return 0; //file does not exist 
	printf("closin..\n");
	fclose(f);
	return 1; 
}


int main(int argc, char** args) 
{ 
	if(argc <1){
		printf("you must pass a port as argumnet\n");
		exit(1);
	}
	
	int port = atoi(args[1]);
	printf("%d %s", port, IP_ADDRESS);

	int udpfd, nready, maxfdp1; 
	char buffer[MAXLINE]; 
	fd_set rset; 
	ssize_t n; 
	socklen_t len; 

	struct sockaddr_in cliaddr, servaddr; 

	/* create UDP socket */
	udpfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	memset(&servaddr, '\0', sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

	
	// binding server addr structure to udp sockfd 
	bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

	// clear the descriptor set 
	FD_ZERO(&rset); 


	for (;;) { 

		// set  udpfd in readset 
		FD_SET(udpfd, &rset); 

		// if udp socket is readable receive the message. 
		if (FD_ISSET(udpfd, &rset)) { 
			len = sizeof(cliaddr); 
			bzero(buffer, sizeof(buffer)); 
			printf("\nMessage from UDP client: "); 
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0, 
						(struct sockaddr*)&cliaddr, &len); 
			puts(buffer);
			//printf("%d number\n", n);
			char filename[100];
			strncpy(filename, buffer,100);
			filename[strcspn(filename, "\r\n")] =0; // get rid of new line
			if(isFileInDirectory(filename)==0){ //if file doesnt exist in dir, we process the info
			printf("file not in dir \n");
				} 
			else {// else we simply wait for another message
				
				printf("File already exists on server, try another file\n");				
			}
			
			//sendto(udpfd, (const char*)message, sizeof(buffer), 0, 
			//	(struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
		} 
	} 
} 
