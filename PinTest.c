#include <wiringPi.h>
#include <stdio.h>

int main (void)
{
	wiringPiSetup();
	pinMode (0, INPUT);
	pinMode (1, INPUT);
	while(1)
	{
		if(digitalRead(0) == LOW)
		{
			printf("0 is low\n");
		}
		if(digitalRead(0) == HIGH)
		{
			printf("0 is high\n");
		}
		if(digitalRead(1) == LOW)
		{
			printf("1 is low\n");
		}
		if(digitalRead(1) == HIGH)
		{
			printf("1 is high\n");
		}
	}
printf("Done \n");
return 0;
}
