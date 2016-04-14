#include <stdio.h>
#include <wiringPi.h>

char* data;

int handshake(void)
{
/*
this function checks for the handshake which should be recieved over the pin
The handshake is used to calculate the time between high and low
The function return the time it has taken for the total handshake divided by four
This should be the time that it takes for a pin to change from high to low
*/
	printf("doing handshake \n");
	int time = 0;
	int handshake = 0;
	int counter = 0;
	while(handshake == 0)
	{
		if(digitalRead(1) == HIGH)
		{
			counter++;
		}
		if(digitalRead(1) == LOW && counter == 1)
		{
			counter++;
		}
		if(digitalRead(1) == HIGH && counter == 2)
		{
			counter++;
		}
		if(digitalRead(1) == LOW && counter == 3)
		{
			handshake = 1;
		}
		time++;
		delayMicroseconds(1);
	printf("round : %d \n", time);
	}
	printf("handshake done \n");
	printf("timing is: %d \n", time);
	return time/4;
}

char* getFromData(int x)
{
/*
This fucntion get 5 characters from the data char pointer
The input for the function is a int which is used to determine were in the char pointer it should gather the data
*/
	char* returnData;
	int nummerData = x+5;
	for(; x < nummerData; x++)
	{
		returnData = returnData + data[x];
	}
	return returnData;
}

void updateFile(void)
{
*
This function is used to put the data from the data char pointer to the data.txt file
It calls the getFromData function to know where every variable is en uses that to put every variable on its own line in the file
*/
	char *lines[11];
	int i;
	for(i = 0; i < 11; i++)
	{
		lines[i] = getFromData(i*5);
	}
	FILE *dataFile;
	dataFile = fopen("data.txt","w");
	for(i = 0; i < 11; i++)
	{
		fprintf(dataFile, "%s\n",lines[i]);
	}
	fclose (dataFile);
}

void gatherData(void)
{
/*
This function is used to gather the data from the TM4C123GXL through the gpio pins.
It puts all the data it gathers in the data char pointer
*/
	printf("data gathering \n");
	int i;
	int k;
	char c[59];
	digitalWrite(0,LOW);
	for(i = 0; i < 59; i++)
	{
		c[i] = '\0';
	}
	int time;
	for(i = 0; i < 59; i++)
	{
		time = handshake();
		for(k = 0; k < 8; k++)
		{
			c[i] |= (digitalRead(1) << k);
			delayMicroseconds(time);
		}
		data = data + c[i];
	}
	printf("data gathering compleet \n");
}

int main (void){
	wiringPiSetup (); //initialise writing to gpio pin
	pinMode (0, OUTPUT);
	pinMode (1, INPUT);

	digitalWrite(0,HIGH);
	delay(1);
	digitalWrite(0,LOW);
	gatherData();
	updateFile();
	printf("%s\n",data);
	while(1)
	{
	}
return 0;
}
//gcc -Wall -o Pins Pins.c -lwiringPi
