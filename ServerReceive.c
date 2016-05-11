#include <mysql.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


MYSQL* conn;
char* server = "localhost";
char* user = "root";
char* password = "raspberry";
char* database = "data";
int fd;

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

int writeDataToDatabase(int deviceID,char* dataValue,char* datatype)
{
	char data[562];
	sprintf(data,"INSERT INTO data(data,datatype) VALUES(%s,'%s');",dataValue,datatype );
	char* Querry = data;
	if(mysql_query(conn,Querry)){
		printf("Error input data\n");
		return 1;
	}
	return 0;
}

int writeDeviceToDatabase(char* deviceIP,char* extensions)
{
	char data[562];
	sprintf(data,"INSERT INTO device(deviceIP,extensions) VALUES('%s','%s');",deviceIP,extensions);
	char* Querry = data;
	if(mysql_query(conn,Querry)){
		printf("Error input data\n");
		return 1;
	}
	return 0;
}

int getNextDeviceID()
{
	char data[562];
	char* deviceInD = 0;
	sprintf(data,"SELECT deviceID FROM device ORDER BY deviceID ASC LIMIT 1;");
	char* Querry = data;
	if(mysql_query(conn,Querry)){
		printf("Error input data\n");
		return -1;
	}
	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) {
		printf("error getting data\n");
		return -1;
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	row = mysql_fetch_row(result);
	//while ((row = mysql_fetch_row(result))) 
	//{ 
      //for(int i = 0; i < num_fields; i++) 
      //{ 
          deviceInD= row[0];// ? row[i] : "NULL"; 
      //} 
	//}
	return atoi(deviceInD) + 1;
}


void func( char* a ) {
	
	
}

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
	return  *buffer;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno = 51717, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char* data = " ";

    printf( "using port #%d\n", portno );
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
		printf("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( portno );
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		printf( "ERROR on binding" );
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
  
    //--- infinite wait on a connection ---
    while ( 1 ) {
		printf( "waiting for new client...\n" );
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
			printf("ERROR on accept");
        printf( "opened new communication with client\n" );
		
		
        while ( data == " " ) {
            //---- wait for a number from client ---
            data = getData( newsockfd );
            printf( "got %s\n", data );
		}
		int deviceID;
		int deviceIDRecieved;
		char deviceIDR[4];
		char* deviceIDre;
		int i,j;
		for(i=0;i<4;i++)
		{
			deviceIDR[i] = data[i];
		}
		deviceIDre = deviceIDR;
		if(atoi(deviceIDre) > 0){
			
			deviceID =getNextDeviceID();
			char IPadres[15];

			j=0;
			for(;i<19;i++)
			{
				IPadres[j]=data[i];
				j++;
			}
			char* ipadres = IPadres;
			j=0;
			int len = strlen(data);
			char extensions[400];
			for(;i<419;i++){
				extensions[j] = data[i];
				if(i == strlen){break;}
				j++;
			}
			char* Extensions = extensions;
			if((writeDeviceToDatabase(ipadres,Extensions);) == 1)
				{
					printf("Error writing to database device");
				}
			sendData( newsockfd,deviceID);
		}
		else
		{
			deviceID = atoi(deviceIDre);
			char dataR[400];
			char dataType[10];
			for(j=0;i<14;i++)
			{
				dataType[j] = data[i];
				j++;
			}
			char* dataTyper = dataType;
			int len = strlen(data);
			for(j=0;i<414;i++)
			{
				dataR[j] = data[i];
				j++;
				if(i == len){break;}
			}
			char* DataR = dataR;
			if((writeDataToDatabase(deviceID,DataR,dataTyper))== 1){printf("Error writing to database data");}
			char* general = "Done";
			sendData(newsockfd,general);
		}
		close( newsockfd );
    }
    return 0; 
}

