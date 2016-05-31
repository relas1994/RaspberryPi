#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include<mysql.h>
#include<pthread.h> //for threading , link with lpthread

MYSQL* conn;
char* server = "localhost";
char* user = "root";
char* password = "raspberry";
char* database = "data";

int i;

int initDatabaseConnection()
{
	conn= mysql_init(NULL);
	printf("start database connection\n");
	if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
		printf("error\n");
		return 1;
	}
	printf("done database connection\n");
	return 0;
}

int writeToDatabaseData(char* dataValue,char* datatype, int deviceID)
{
	if(initDatabaseConnection() == 1)
	{
		return 1;
		printf("error database connection");
	}
	char data[562];
	sprintf(data,"INSERT INTO data(deviceID,data,datatype) VALUES(%d,%s,'%s');",deviceID,dataValue,datatype );
	char* Querry = data;
	if(mysql_query(conn,Querry)){
		printf("Error input data\n");
		return 1;
	}
	mysql_close(conn);
	return 0;
}

int writeToDatabaseDevice(char* deviceIP,char* extensions)
{
	if(initDatabaseConnection() == 1)
	{
		printf("error database connection");
		return 1;
	}
	char data[562];
	sprintf(data,"INSERT INTO device(deviceIP,extensions) VALUES('%s','%s');",deviceIP,extensions);
	char* Querry = data;
	if(mysql_query(conn,Querry)){
		printf("Error input data\n");
		return 1;
	}
	mysql_close(conn);
	return 0;
}

int getDeviceIDFromDatabase()
{
	if(initDatabaseConnection() == 1)
	{
		printf("error database connection\n");
		return 1;
	}
	MYSQL_RES *res;
	MYSQL_ROW row;
	char data[562] = "/0";
	char* deviceidnr;
	int i;
	sprintf(data,"SELECT deviceID FROM device ORDER BY deviceID ASC LIMIT 1;");
	char* Querry = data;
	if(mysql_query(conn,Querry)){
		printf("Error input data\n");
		return 1;
	}
	printf("query send\n");
	res = mysql_store_result(conn);
	int num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		for(i=0;i<num_fields;i++)
		{
			deviceidnr = row[i] ? row[i] : "NULL";
			printf("deviceidnr: %s\n",deviceidnr);
		}

	}
	int nr = atoi(deviceidnr)+1;
	printf("%d\n",nr);
	mysql_free_result(res);
	mysql_close(conn);
	return nr;
}

void messageClient(char* message,int new_socket){
	write(new_socket , message , strlen(message));
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    	int sock = *(int*)socket_desc;
    	int read_size;
    	char client_message[2000];
	int deviceID;
	char* clientIP;
	char* extensions;
	char* data;
	char* dataType;
	if ((read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    	{
		printf("reading\n");
	}
	if(client_message[0] == 'I' && client_message[1] == 'D')
	{
		printf("ID\n");
		char message[50];
		deviceID = getDeviceIDFromDatabase();
		printf("%d",deviceID);
		sprintf(message,"%d",deviceID);
		messageClient(message, sock);
		printf("ID done");
	}
	else if(client_message[0] == 'I' && client_message[1] == 'P')	
	{
		printf("IP\n");
		char clientip[15];
		for(i=0;i<15;i++)
		{
			clientip[i] = client_message[i+2];
		}
		clientIP = clientip;
		char clientMessage[2000];
		for(i=0;i<2000;i++)
		{
			clientMessage[i] = client_message[i+17];
		}
		extensions = clientMessage;
		writeToDatabaseDevice(clientIP,extensions);
		messageClient("IP received", sock);
	}
	else if(client_message[0] == 'D' && client_message[1] == 'A')
	{
		printf("Data\n");
		char deviceid[4] = {'0','0','0','0'};
		char dataA[2000];
		char dataTypeA[2000];
		for(i=0;i<4;i++)
		{
			deviceid[i] = client_message[i+2];
		}
		for(i=0;i<2000;i++)
		{
			if(client_message[i+6] == '/')
			{
				break;
			}
			dataA[i] = client_message[i+6];
		}
		data = dataA;
		i++;
		for(;i<2000;i++)
		{
			dataTypeA[i] = client_message[i+6];
		}
		dataType = dataTypeA;
		writeToDatabaseData(data,dataType,atoi(deviceid));
		messageClient("Data received", sock);
	}

    	if(read_size == 0)
    	{
        	printf("Client disconnected\n");
	        fflush(stdout);
	}
	else if(read_size == -1)
	{
		printf("recv failed\n");
	}
	//Free the socket pointer
	free(socket_desc);
	return 0;
}

int main(int argc , char *argv[])
{
	int nr = getDeviceIDFromDatabase();
	int error = writeToDatabaseDevice("1.1.1","test");
	error = writeToDatabaseData("42","test",nr);
	int socket_desc , new_socket , c , *new_sock;
    	struct sockaddr_in server , client;
    	char *message;
    	//Create socket
    	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    	if (socket_desc == -1)
    	{
        	printf("Could not create socket\n");
    	}
    	//Prepare the sockaddr_in structure
    	server.sin_family = AF_INET;
    	server.sin_addr.s_addr = INADDR_ANY;
    	server.sin_port = htons( 8888 );
    	//Bind
    	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	printf("bind failed\n");
        	return 1;
    	}
    	printf("bind done\n");
    	//Listen
    	listen(socket_desc , 3);
    	//Accept and incoming connection
    	printf("Waiting for incoming connections\n");
    	c = sizeof(struct sockaddr_in);
    	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    	{
        	printf("Connection accepted\n");
        	//Reply to the client
        	message = "connection accepted";
        	messageClient(message,new_socket);
        	pthread_t sniffer_thread;
        	new_sock = malloc(1);
        	*new_sock = new_socket;
        	if( pthread_create( &sniffer_thread , NULL , connection_handler , (void*) new_sock) < 0)
        	{
            		printf("could not create thread\n");
            		return 1;
        	}
        	//Now join the thread , so that we dont terminate before the thread
        	//pthread_join( sniffer_thread , NULL);
        	printf("Handler assigned\n");
    	}
    	if (new_socket<0)
    	{
        	printf("accept failed\n");
        	return 1;
    	}
return 0;
}

