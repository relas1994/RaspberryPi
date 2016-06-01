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

char *deviceID= "\0";
char* clientIP = "192.168.134.246";
char* extensions = "None";

void sendData( int sockfd, char* x ) {
  	int n;
  	char buffer[32];
  	sprintf( buffer, "%s\n", x );
  	if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
	{
      		printf( "ERROR writing to socket");
	}
	printf("Done sending\n");
}

char* getData( int sockfd )
{
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
	char *messageR;
    int data;
	int device = 0;
	char* deviceID = "test";
	char* servermsg = "test";
    if (argc < 3) 
	{
     	printf( "contacting %s on port %d\n", serverIp, portno );
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
	while(deviceID == 0)
	{}
	device = atoi(deviceID);
	printf("%d\n",device);
	
	sendData(sockfd,"IP192.168.134.246/test/");
	servermsg = getData(sockfd);	
	while(servermsg == "test")
	{}
	printf("%s\n",servermsg);
	
	char mes[500];
	sprintf(mes,"DA%d/42/hi/",device);
	printf("%s\n",mes);
	sendData(sockfd,mes);

	while(messageR != "Data received")
	{}	
    close( sockfd );
    return 0;
}
