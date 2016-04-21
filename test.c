#include <stdio.h>
#include <time.h>

int main(void){
	struct timespec time;
	clock_gettime(CLOCK_REALTIME,&time);
	int year = time.tv_sec/31536000 + 1970;
	int day = (time.tv_sec - (time.tv_sec/31536000-1970))/84600;
	printf("year: %d  day: %d \n",year);
	return 0;
}
