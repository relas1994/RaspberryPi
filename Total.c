#include <stdio.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <mysql.h>


char data[4096];
char indentifierchars[52] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
MYSQL* conn;
char* server = "localhost";
char* user = "root";
char* password = "raspberry";
char* database = "data";

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
void getExtension(){

}

void init()
{
	printf("start init \n");
	int i;
	for(i=0;i<4096;i++)
	{
		data[i] = ' ';
	}
	if(initDatabaseConnection() == 1)
	{
		printf("error setting up database connection\n");
	}
	printf("init done\n");
	
}

//data should be starting with indentifier of 5 chars followed by 5 digits
void getData(){
	int i = 0;
	int fd = serialOpen("/dev/ttyACM0",9600);

	if(fd < 0){printf("error connecting to serial device\n");}
	while(serialDataAvail(fd) >= 1)
	{
		data[i] = serialGetchar(fd);
	}
}

int writeToDatabase(char* dataValue,char* datatype)
{
	char data[562];
	sprintf(data,"INSERT INTO data(data,datatype) VALUES(%s,'%s');",dataValue,datatype );
	char* Querry = data;
	if(mysql_query(conn,Querry)){
		printf("Error input data");
		return 1;
	}
	return 0;
}

void getDataFromDataArray(){
	getData();
	int j,k;
	int check = 1;
	int i = 0;
	for(j=0;j<4096;j++)
	{
		if(check == 0){
			char singleDataInd[5] = {' ',' ',' ',' ',' '};
			char singleDataVal[5] = {' ',' ',' ',' ',' '};
			for(k=0;k<5;k++)
			{
				singleDataInd[k] = data[i];
				i++;
			}
			for(k=0;k<5;k++)
			{
				singleDataVal[k] = data[i];
				i++;
			}
			char* dataI = singleDataInd;
			char* dataV = singleDataVal;
			writeToDatabase(dataI,dataV);
		}
		for(k=0;k<52;k++)
		{
			if(data[i] != indentifierchars[k])
			{
				check = 1;
				i++;
			}
			else
			{
				check = 0;
			}
		}
	}
}

int main(void)
{
	init();
	getDataFromDataArray();
	printf("done\n");
	return 0;
}
