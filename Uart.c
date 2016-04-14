#include <stdio.h>
#include <wiringSerial.h>
#include <wiringPi.h>

char data[12][5];
int requestPin;

int getData(void)
{
	int fd = serialOpen("/dev/ttyACM0",9600);
	if(fd < 0){printf("error connecting to serial device\n");return 0;}
	if(requestPin == 0){serialPuts(fd,"B");data[requestPin][0]='B';serialPuts(fd,"4");data[requestPin][1]='4';}
	if(requestPin == 1){serialPuts(fd,"B");data[requestPin][0]='B';serialPuts(fd,"5");data[requestPin][1]='5';}
	if(requestPin == 2){serialPuts(fd,"D");data[requestPin][0]='D';serialPuts(fd,"0");data[requestPin][1]='0';}
	if(requestPin == 3){serialPuts(fd,"D");data[requestPin][0]='D';serialPuts(fd,"1");data[requestPin][1]='1';}
	if(requestPin == 4){serialPuts(fd,"D");data[requestPin][0]='D';serialPuts(fd,"2");data[requestPin][1]='2';}
	if(requestPin == 5){serialPuts(fd,"D");data[requestPin][0]='D';serialPuts(fd,"3");data[requestPin][1]='3';}
	if(requestPin == 6){serialPuts(fd,"E");data[requestPin][0]='E';serialPuts(fd,"0");data[requestPin][1]='0';}
	if(requestPin == 7){serialPuts(fd,"E");data[requestPin][0]='E';serialPuts(fd,"1");data[requestPin][1]='1';}
	if(requestPin == 8){serialPuts(fd,"E");data[requestPin][0]='E';serialPuts(fd,"2");data[requestPin][1]='2';}
	if(requestPin == 9){serialPuts(fd,"E");data[requestPin][0]='E';serialPuts(fd,"3");data[requestPin][1]='3';}
	if(requestPin == 10){serialPuts(fd,"E");data[requestPin][0]='E';serialPuts(fd,"4");data[requestPin][1]='4';}
	if(requestPin == 11){serialPuts(fd,"E");data[requestPin][0]='E';serialPuts(fd,"5");data[requestPin][1]='5';}
	int j;
	for(j = 2; j < 5; j++)
	{
		data[requestPin][j] = serialGetchar(fd);
	}
	return 1;
}

void printDataToFile(void)
{
	int j;
	FILE* fp;
	fp = fopen("data.txt","w");
	for(requestPin = 0; requestPin < 12; requestPin++)
	{
		if(getData()== 0){break;}
		j=0;
		while(j<5)
		{
			if(j == 2){fprintf(fp," ");}
			fputc(data[requestPin][j],fp);
			if(serialDataAvail > 0){j++;}
		}
		fprintf(fp,"\n");
	}
}


int main(void)
{
	printDataToFile();
	printf("done\n");
	return 0;
}
