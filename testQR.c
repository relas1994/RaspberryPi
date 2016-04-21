#include <qrencode.h>
#include <stdio.h>

int main(void){
	QRcode_encodeString("192.168.134.102",0,QR_ECLEVEL_L,QR_MODE_AN ,0);
	return 0;
}
