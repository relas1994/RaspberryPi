/* A simple client program to interact with the myServer.c program on the Raspberry.
myClient.c
D. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Macbook laptop as follows:
   
    g++ -o myClient myClient.c 
    ./myClient


*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 

void sendData( int sockfd, char* x ) {
  	int n;
	printf("Send data\n");
	printf("%s\n",x);
  	if ( (n = send( sockfd, x, strlen(x),0 ) ) < 0 )
	{
      		printf( "ERROR writing to socket");
	}
	printf("Done sending\n");
}

char* getData( int sockfd ) {
	printf("get Data\n");
  	char buffer[2000];

  	if ( (recv(sockfd,buffer,2000,0) ) < 0 )
	{
       		printf( "ERROR reading from socket");
	}
	char* message = buffer;
	printf("done receiving\n");
  	return message;
}

int main(int argc, char *argv[])
{
	char deviceid[100];
    	int sockfd, portno = 8888, n;
    	char serverIp[] = "192.168.1.103";
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
    	char buffer[256];
    	int data;
	int device = 0;
	char* deviceID = "test";
	char* servermsg = "test";
    	if (argc < 3) {
      		// error( const_cast<char *>( "usage myClient2 hostname port\n" ) );
      		printf( "contacting %s on port %d\n", serverIp, portno );
      		// exit(0);
    	}
    	if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
	{
        	printf( "ERROR opening socket");
	}

    	if ( ( server = gethostbyname( serverIp ) ) == NULL )
	{
        	printf("ERROR, no such host\n");
	}
    	bzero( (char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    	serv_addr.sin_port = htons(portno);
    	if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{
        	printf( "ERROR connecting");
	}

	sendData(sockfd,"ID/");
	deviceID = getData(sockfd);
	printf("%s\n",deviceID);
	device = atoi(deviceID);
	printf("%d\n",device);
	while(deviceID == 0)
	{}
	sendData(sockfd,"IP192.168.134.246/test/");
	servermsg = getData(sockfd);
	printf("%s\n",servermsg);
	while(servermsg == "test")
	{}
	char mes[500];
	sprintf(mes,"DA%d/42/hi/",device);
	printf("%s\n",mes);
	sendData(sockfd,mes);
    	close( sockfd );
    	return 0;
}
