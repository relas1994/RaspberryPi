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
		printf("error database connection\n");
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
		printf("error database connection\n");
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
	sprintf(data,"SELECT deviceID FROM device ORDER BY deviceID DESC LIMIT 1;");
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
		}

	}
	int nr = atoi(deviceidnr)+1;
	mysql_free_result(res);
	mysql_close(conn);
	return nr;
}

void messageClient(char* message,int new_socket){
	write(new_socket , message , strlen(message));
}

int main(int argc , char *argv[])
{
//	int nr = getDeviceIDFromDatabase();
//	int error = writeToDatabaseDevice("1.1.1","test");
//	error = writeToDatabaseData("42","test",nr);
	int i;
	int read_size;
	char client_message[2000];
	int deviceID;
	char* clientIP;
	char* extensions;
	char* dataType;

	int socket_desc , new_socket , c;
    	struct sockaddr_in server , client;
    	char *message;

    	//Create socket
    	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    	if (socket_desc == -1)
    	{
        	printf("Could not create socket");
    	}

    	//Prepare the sockaddr_in structure
    	server.sin_family = AF_INET;
    	server.sin_addr.s_addr = INADDR_ANY;
    	server.sin_port = htons( 8888 );

    	//Bind
    	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    	{
        	printf("bind failed");
        	return 1;
    	}
    	printf("bind done");

    	//Listen
    	listen(socket_desc , 3);

    	//Accept and incoming connection
    	printf("Waiting for incoming connections...");
    	c = sizeof(struct sockaddr_in);
    	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    	{
        	while(1){
                     	for(i=0;i<2000;i++)
                        {
                        	client_message[i] = ' ';
                      	}

			if ((recv(new_socket , client_message , 2000 , 0)) > 0 )
    			{
				printf("reading\n");
			}
			if(client_message[0] == 'I' && client_message[1] == 'D')
			{
				printf("ID\n");
				char message[50];
				deviceID = getDeviceIDFromDatabase();
				sprintf(message,"%d",deviceID);
				printf("%s\n",message);
				messageClient(message,new_socket);
				printf("ID done\n");
			}
			else if(client_message[0] == 'I' && client_message[1] == 'P')
			{
				printf("IP\n");
				printf("%s\n",client_message);
				char clientip[15];
				for(i=0;i<15;i++)
				{
					clientip[i] = client_message[i+2];
				}
				printf("%d\n",i);
				clientIP = clientip;
				printf("%s\n",clientIP);
				char clientMessage[2000];
				for(i=0;i<2000;i++)
				{
					if(client_message[i+17] == '/')
					{
						break;
					}
					else
					{
						clientMessage[i] = client_message[i+17];
					}
				}
				clientMessage[i] = ' ';
				extensions = clientMessage;
				printf("%s\n",extensions);
				printf("%d\n",i);
				writeToDatabaseDevice(clientIP,extensions);
                              	messageClient("IP received", new_socket);
        		}
			else if(client_message[0] == 'D' && client_message[1] == 'A')
			{
				printf("Data\n");
				printf("%s\n",client_message);
				char deviceid[4];
				char* deviceId;
				char dataA[2000];
				for(i=0;i<2000;i++)
				{
					dataA[i] = ' ';
				}
				char* data;
				char dataTypeA[2000];
				for(i=0;i<4;i++)
				{
					if(client_message[i+2] == '/')
					{
						break;
					}
					else
					{
						deviceid[i] = client_message[i+2];
					}
				}
				deviceId = deviceid;
				i++;
				printf("deviceid: %s\n",deviceId);
				for(;i<2000;i++)
				{
					if(client_message[i+2] == '/')
					{
						printf("i: %d\n",i);
						break;
					}
					else
					{
						dataA[i] = client_message[i+2];
					}
				}
				data = dataA;
				i++;
				printf("data: %s\n",data);
				for(;i<2000;i++)
				{
					if(client_message[i+2] = '/')
					{
						printf("i: %d\n",i);
						break;
					}
					else
					{
						dataTypeA[i] = client_message[i+2];
						printf("dataTypeA[%d]:%c\n",i,dataTypeA[i]);
					}
				}
				dataType = dataTypeA;
				printf("dataType: %s\n",dataType);
				writeToDatabaseData(data,dataType,atoi(deviceid));
                                messageClient("Data received", new_socket);
			}
    		}
	}
	if (new_socket<0)
    	{
       		 printf("accept failed");
        	return 1;
    	}
return 0;
}

