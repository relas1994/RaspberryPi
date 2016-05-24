#include <stdio.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <mysql.h>

char dataTM4[12][5];
char data[4096];
char indentifierchars[52] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
MYSQL* conn;
char* server = "localhost";
char* user = "root";
char* password = "raspberry";
char* database = "data";
int fd;
char extensionID[10];
char* extension = " ";
int requestPin;

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

int initUart()
{
	int i = 0;
	fd = serialOpen("/dev/ttyACM0",9600);
	if(fd < 0){return 1;}
	serialPutchar(fd,'t');
	delay(100);
	while(extension == " "){
		while(serialDataAvail(fd) >= 1)
		{
			extensionID[i] = serialGetchar(fd);
			if(i == 10){
				break;
			}
			i++;
		}
		extension = extensionID;
	}
	for(i=0;i<10;i++){printf("%c",extensionID[i]);}
	printf("\n%s \n",extension);
	return 0;
}

void initArrays(){
	int i,j;
	for(i=0;i<4096;i++)
	{
		data[i] = ' ';
	}
	for(i=0;i<10;i++)
	{
		extensionID[i] = ' ';
	}
	for(i=0;i<12;i++)
	{
		for(j=0;j<5;j++)
		{
			dataTM4[i][j] = ' ';
		}
	}
}

void init()
{
	printf("start init \n");
	initArrays();
	if(initDatabaseConnection() == 1)
	{
		printf("error setting up database connection\n");
	}
	if(initUart() == 1)
	{
		printf("error connecting to serial device\n");
	}
	printf("init done\n");
}

void getData(){
if(extensionID[0] == 'T' && extensionID[1] == 'M' && extensionID[2] == '4')
    	{
        	for(requestPin = 0; requestPin < 12; requestPin++){
			switch(requestPin){
                	case 0:
				serialPuts(fd,"B");
				dataTM4[requestPin][0]='B';
				serialPuts(fd,"4");									dataTM4[requestPin][1]='4';
				break;
			case 1:
				serialPuts(fd,"B");
				dataTM4[requestPin][0]='B';
				serialPuts(fd,"5");
				dataTM4[requestPin][1]='5';
				break;
			case 2:
				serialPuts(fd,"D");
				dataTM4[requestPin][0]='D';
				serialPuts(fd,"0");
				dataTM4[requestPin][1]='0';
				break;
			case 3:
				serialPuts(fd,"D");
				dataTM4[requestPin][0]='D';
				serialPuts(fd,"1");
				dataTM4[requestPin][1]='1';
				break;
			case 4:
				serialPuts(fd,"D");
				dataTM4[requestPin][0]='D';
				serialPuts(fd,"2");
				dataTM4[requestPin][1]='2';								break;
			case 5:
				serialPuts(fd,"D");
				dataTM4[requestPin][0]='D';
				serialPuts(fd,"3");
				dataTM4[requestPin][1]='3';
				break;
			case 6:
				serialPuts(fd,"E");
				dataTM4[requestPin][0]='E';
				serialPuts(fd,"0");
				dataTM4[requestPin][1]='0'; 								break;
			case 7:
				serialPuts(fd,"E");
				dataTM4[requestPin][0]='E';
				serialPuts(fd,"1");
				dataTM4[requestPin][1]='1'; 								break;
			case 8:            
				serialPuts(fd,"E");
				dataTM4[requestPin][0]='E';
				serialPuts(fd,"2");
				dataTM4[requestPin][1]='2';
				break;
			case 9:            
				serialPuts(fd,"E");
				dataTM4[requestPin][0]='E';
				serialPuts(fd,"3");
				dataTM4[requestPin][1]='3'; 								break;
			case 10:            
				serialPuts(fd,"E");
				dataTM4[requestPin][0]='E';
				serialPuts(fd,"4");
				dataTM4[requestPin][1]='4'; 								break;
			case 11:            
				serialPuts(fd,"E");
				dataTM4[requestPin][0]='E';
				serialPuts(fd,"5");
				dataTM4[requestPin][1]='5'; 								break;
			}
	 		int j;
			for(j = 2; j < 5; j++)
			{
      	          	dataTM4[requestPin][j] = serialGetchar(fd);
	            }
        	}
		printf("collection complete \n");
    	}
    	else
    	{
        	int i = 0;
        	while(serialDataAvail(fd) >= 1)
        	{
            	data[i] = serialGetchar(fd);
            	i++;
        	}
    	}
}


int writeToDatabase(char* dataValue,char* datatype)
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

void getDataFromDataArray()
{
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

void sendDataTM4CDatabase()
{
	char Indent[2];
	char Data[3];
	char* pID = "test";
	char* pData = "test";
	int i,j;
	for(i=0;i<12;i++)
	{
		Indent[0] = dataTM4[i][0];
		Indent[1] = dataTM4[i][1];
		Data[0] = dataTM4[i][2];
		Data[1] = dataTM4[i][3];
		Data[2] = dataTM4[i][4];
		pID = Indent;
		pData = Data;
		printf("%s\n",pID);
		printf("%s\n",pData);
		writeToDatabase(pData,pID);
	}
}

int main(void)
{
	init();
	getData();
	if(extensionID[0] == 'T' && extensionID[1] == 'M' && extensionID[2] == '4')
	{
		sendDataTM4CDatabase();
	}
	else
	{
		getDataFromDataArray();
	}
	printf("done\n");
	return 0;
}
