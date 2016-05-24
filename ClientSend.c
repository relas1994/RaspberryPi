#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<string.h>    //strlen

int SendDataServer()
{
	int socket_desc;
    	struct sockaddr_in server;
    	char *message;

    	//Create socket
    	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    	if (socket_desc == -1)
    	{
        	printf("Could not create socket");
    	}

    	server.sin_addr.s_addr = inet_addr("192.168.1.103");
    	server.sin_family = AF_INET;
    	server.sin_port = htons( 8888 );

    	//Connect to remote server
    	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	puts("connect error");
        	return 1;
    	}
    	puts("Connected\n");

    	//Send some data
    	message = "Hallo";
    	if( send(socket_desc , message , strlen(message) , 0) < 0)
    	{
        	puts("Send failed");
		close(socket_desc);
        	return 1;
    	}
    	puts("Data Send\n");
	close(socket_desc);
    	return 0;
}



int main(int argc , char *argv[])
{
	while(1){
		SendDataServer();
	}
}
