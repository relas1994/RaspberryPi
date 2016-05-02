#include <mysql.h>
#include <stdio.h>

int main(void)
{
	FILE *fp;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *user = "root";
	char *password = "raspberry";
	char *database = "data";
	int firstCreation;
	char deviceID[3];
	char* deviceid;
	conn = mysql_init(NULL);

	fp = fopen("DeviceInfo.txt","r+");
	if(fp == NULL)
	{
		fp = fopen("DeviceInfo.txt","w");
		fclose(fp);
		firstCreation = 0;
		fp = fopen("DeviceInfo.txt","r+");
	}

	if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 0;
	}

	if(firstCreation == 0){
  		if(mysql_query(conn,"INSERT INTO deviceInfo(ipAddres) VALUES('192.168.134.153');"))
		{
			printf("Error input deviceInfo");
			return 0;
		}
		if(mysql_query(conn,"SELECT MAX(deviceID) FROM deviceInfo;")){
			printf("Error getting device info");
		}

		MYSQL_RES *result = mysql_store_result(conn);

		if (result == NULL)
 		{
			printf("result is null");
 		}

		int num_fields = mysql_num_fields(result);
		MYSQL_ROW row;
		char* deviceIdent;
		while ((row = mysql_fetch_row(result)))
		{
			int i;
			for(i=0;i<num_fields;i++)
			{
				deviceIdent = row[i];
			}
			for(i=0;i<3;i++){
				if(deviceIdent[i]==' '){break;}
				deviceID[i]=deviceIdent[i];
				fputc(deviceID[i],fp);
			}
		}
		deviceid = deviceID;
		firstCreation = 1;
	}
	float dataValue = 3;
	char* datatype = "test";
	if(firstCreation != 0){
		char data[562];
		sprintf(data,"INSERT INTO data(deviceID,data,datatype) VALUES(%s,%f,'%s');",deviceid,dataValue,datatype );
		printf("%s\n",&data);
		char * Querry = data;
		if(mysql_query(conn,Querry)){
			printf("Error input data");
		}
	}
	fclose(fp);
	return 0;
}
