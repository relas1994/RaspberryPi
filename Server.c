#include<stdio.h>
#include<string.h> 
#include<stdlib.h> 
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h> 
#include<math.h>
#include<mysql.h>

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
	printf("%s\n",data);
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
	int i;
	int read_size;
	char client_message[2000];
	int deviceID = 0;
	char* extensions = " ";
	char* dataType = " ";
	char deviceid[4] = {0,0,0,0};
	char* deviceId = " ";
	char dataA[2000];
	char* data = " ";
	char dataTypeA[2000];	
	char clientMessage[2000];
    char clientip[15];
    int firstBreak;
    int secondBreak;
	int thirdBreak;
	int j;

	for(i=0;i<2000;i++)
	{
		client_message[i] = 0;
		dataA[i] = 0;
		dataTypeA[i] = 0;
		clientMessage[i] = 0;
	}
	int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char *message;

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
       	printf("Could not create socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    if( bind(socket_desc,(struct socka	ddr *)&server , sizeof(server)) < 0)
    {
       	printf("bind failed");
       	return 1;
    }
    printf("bind done");

    listen(socket_desc , 3);
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
				for(i=2;i<2000;i++)
				{
					if(client_message[i] == '/')
					{
						firstBreak = i;
						break;
					}
				}
				printf("i:%d\n",firstBreak);
				i++;
				for(;i<2000;i++)
				{
					if(client_message[i] == '/')
					{
						secondBreak = i;
						break;
					}
				}
				printf("i:%d\n",secondBreak);
				j = 0;
				for(i=2;i<firstBreak;i++)
				{
					clientip[j] = client_message[i];
					printf("clientip:%c\n",clientip[i]);
					j++;
				}
				i++;
				for(j=0;i<secondBreak;i++)
				{
					clientMessage[j] = client_message[i];
					j++;
				}
				printf("clientIP:%s\n",clientip);
				printf("extensions:%s\n",clientMessage);
				writeToDatabaseDevice(clientip,clientMessage);
		         	messageClient("IP received", new_socket);
        		}
			else if(client_message[0] == 'D' && client_message[1] == 'A')
			{
				printf("Data\n");
				printf("%s\n",client_message);
				for(i=2;i<2000;i++)
				{
					if(client_message[i] == '/')
					{
						firstBreak = i;
						break;
					}
				}
				printf("i:%d\n",firstBreak);
				i++;
				for(;i<2000;i++)
				{
					if(client_message[i] == '/')
					{
						secondBreak = i;
						break;
					}
				}
				printf("i:%d\n",secondBreak);
				i++;
				for(;i<2000;i++)
				{
					if(client_message[i] == '/')
					{
						thirdBreak = i;
						break;
					}
				}
				printf("i:%d\n",thirdBreak);
				j=0;
				for(i=2;i<firstBreak;i++)
				{
					deviceid[j] = client_message[i];
					j++;
				}
				i++;
				for(j=0;i<secondBreak;i++)
				{
					dataA[j] = client_message[i];
					j++;
				}
				i++;
				for(j=0;i<thirdBreak;i++)
				{
					dataTypeA[j] = client_message[i];
					j++;
				}
				printf("deviceID: %s\n",deviceid);
				printf("dataA: %s\n",dataA);
				printf("dataTypeA: %s\n",dataTypeA);
				int device = atoi(deviceid);
				printf("device: %d\n",device);
				writeToDatabaseData(dataA,dataTypeA,device);
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
