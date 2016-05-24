<<<<<<< HEAD
=======
/* A simple client program to interact with the myServer.c program on the Raspberry.
myClient.c
D. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Macbook laptop as follows:
   
    g++ -o myClient myClient.c 
    ./myClient


*/
>>>>>>> 0177a9f92ea5562d24c52a2bf5b18a122280b167
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
<<<<<<< HEAD
#include <arpa/inet.h> 

void sendData( int sockfd, char* x ) {
  	int n;
  	char buffer[32];
  	sprintf( buffer, "%s\n", x );
=======
#include <errno.h>
#include <arpa/inet.h> 

void error(char *msg) {
    perror(msg);
    exit(0);
}

void sendData( int sockfd, int x ) {
  	int n;

  	char buffer[32];
  	sprintf( buffer, "%d\n", x );
>>>>>>> 0177a9f92ea5562d24c52a2bf5b18a122280b167
  	if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
      		printf( "ERROR writing to socket");
  	buffer[n] = '\0';
}

<<<<<<< HEAD
char* getData( int sockfd ) {
  	char buffer[32];
  	int n;
  	if ( (n = read(sockfd,buffer,31) ) < 0 )
       		printf( "ERROR reading from socket");
  	buffer[n] = '\0';
  	return &buffer;
=======
int getData( int sockfd ) {
  	char buffer[32];
  	int n;

  	if ( (n = read(sockfd,buffer,31) ) < 0 )
       		printf( "ERROR reading from socket");
  	buffer[n] = '\0';
  	return atoi( buffer );
>>>>>>> 0177a9f92ea5562d24c52a2bf5b18a122280b167
}

int main(int argc, char *argv[])
{
    	int sockfd, portno = 8888, n;
    	char serverIp[] = "192.168.1.103";
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
    	char buffer[256];
<<<<<<< HEAD
		char *deviceID= "\0";
		char *messageR;
=======
>>>>>>> 0177a9f92ea5562d24c52a2bf5b18a122280b167
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

<<<<<<< HEAD
		
		while(deviceID == "\0")
		{
			sendData(sockfd,"ID");
			deviceID = getData(sockfd);
		}
		char message[2000];
		char* clientIP = "192.168.134.246";
		char* extensions = "None";
		sprintf(message,"IP%s%s",clientIP,extensions);
		while(messageR != "IP received")
		{
			sendData(sockfd,&message);
			messageR = getData(sockfd);
		}
		
		
		
		while(messageR != "Data received")
		{
			
		}
		
		sendData(sockFD,);
		
    	close( sockfd );
    	return 0;
}
=======
    	for ( n = 0; n < 10; n++ ) {
      		sendData( sockfd, n );
      		data = getData( sockfd );
      		printf("%d ->  %d\n",n, data );
    	}
    	sendData( sockfd, -2 );

    	close( sockfd );
    	return 0;
}
>>>>>>> 0177a9f92ea5562d24c52a2bf5b18a122280b167
