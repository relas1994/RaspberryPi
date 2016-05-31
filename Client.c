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
      		printf( "ERROR writing to socket");
  	buffer[n] = '\0';
}

char* getData( int sockfd ) {
  	char buffer[32];
  	int n;
  	if ( (n = read(sockfd,buffer,31) ) < 0 )
       		printf( "ERROR reading from socket");
  	buffer[n] = '\0';
  	return &buffer;
}

void initSetup(int sockfd)
{
	while(deviceID == "\0")
	{
		sendData(sockfd,"ID");
		deviceID = getData(sockfd);
	}
	char message[2000];
	sprintf(message,"IP%s%s",clientIP,extensions);
	while(messageR != "IP received")
	{
		sendData(sockfd,&message);
		messageR = getData(sockfd);
	}
}

void sendData(int sockfd,char* data, char* dataType)
{
	char message[2000];
	sprintf(message,"DA%s/%s",data,dataType);
	while(messageR!= "Data received")
	{
		sendData(sockfd,&message);
		messageR = getData(sockfd);
	}
}

int main(int argc, char *argv[])
{
    	int sockfd, portno = 8888, n;
    	char serverIp[] = "192.168.1.103";
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
    	char buffer[256];
		char *messageR;
    	int data;

    	if (argc < 3) {
      		// error( const_cast<char *>( "usage myClient2 hostname port\n" ) );
      		printf( "contacting %s on port %d\n", serverIp, portno );
      		// exit(0);
    	}
    	if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
        	printf( "ERROR opening socket");

    	if ( ( server = gethostbyname( serverIp ) ) == NULL ) 
        	printf("ERROR, no such host\n");
    
    	bzero( (char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    	serv_addr.sin_port = htons(portno);
    	if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        	printf( "ERROR connecting");

		while(messageR != "Data received")
		{
			
		}
		
		sendData(sockFD,);
		
    	close( sockfd );
    	return 0;
}