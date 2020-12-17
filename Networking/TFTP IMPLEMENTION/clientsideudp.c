// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> //sockaddr_in, sin_family, sin_port, sin_addr.s_addr
#include <sys/socket.h> //connection between the client and the server 
#include <arpa/inet.h> //The <arpa/inet.h> header makes available the type in_port_t and the type in_addr_t as defined in the description of <netinet/in.h>.
#include <netinet/in.h> //The <netinet/in.h> header shall define the sockaddr_in 

#define PORT	 8080 
#define MAXLINE 1024 //1024 chars

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) != 0 ) { //the socket is not opend
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); //initliaize the servaddr, put 0 to the the struct.
	
	// Filling server information 
	servaddr.sin_family = AF_INET; //IPV4 
	servaddr.sin_port = htons(PORT); //convert 8080 from string to hex(/0x1F90)
	servaddr.sin_addr.s_addr = INADDR_ANY; //LOCAL ADDRESS.
	
	int n, len; 
	
	sendto(sockfd, (const char *)hello, strlen(hello), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); //CONST=MORE SECURE.
	printf("Hello message sent.\n"); 
		
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, //n= size of the buffer that recieved= is happend. buffer= saves the data that recieved from the server
				MSG_WAITALL, (struct sockaddr *) &servaddr, 
				&len); 
	
	//if (n==0) //true
	//while (1)
	
	buffer[n] = '\0'; 
	printf("Server : %s\n", buffer); 

	close(sockfd); 
	return 0; 
} 
