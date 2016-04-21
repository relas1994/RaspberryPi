#include <mysql.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *user = "root";
	char *password = "raspberry";
	char *database = "data";
	conn = mysql_init(NULL);

	if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 0;
	}
	double x = 5;
	char p  = 'x';
	int y = 0;
	if(mysql_query(conn,"INSERT INTO data Values(0,1,'test',0);")){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return 0;
	}
	return 0;
}
