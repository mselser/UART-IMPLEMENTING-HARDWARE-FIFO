#include "App.h"

#include "UART.h"
#include "PORT.h"
#include "SysTick.h"

//char data[10] = {'A','B','C','D','E','F','G','H','I','J'};

void send_UART_Package(unsigned int rotate_r, unsigned int rotate_f, unsigned char id, char * data );

static unsigned int rotate_r = 0;
static unsigned int rotate_f = 0;
static unsigned char id = 1;

void App_Init (void)
{
	PORT_Init();
	SysTick_Init();
	UART_Init();
}

void App_Run (void)
{
	if(id > 6)
		id = 1;
	if(rotate_r > 366)
		rotate_r= 0;
	if(rotate_f > 360)
		rotate_f = 0;
	char data[8];
	char pepe[] = {'1','2','3','4','5','6','7','8'};
	send_UART_Package(rotate_r,rotate_f,id,data);
	id++;
	rotate_r++;
	rotate_f++;
	if(UART_Get_Status())
		UART_Send_Data(data, 8);
}

void send_UART_Package(unsigned int rotate_r, unsigned int rotate_f, unsigned char id, char data[8] ){
	if(rotate_r <= 360 && rotate_f <= 360 && id <= 6 && id > 0){
//		MANDO PAQUETES PARA SINCRONIZACION
		data[0] = 255;
		data[1] = 255;

//		MANDO ID
		data[2] = id;

// 		MANDO ROTATE_R
		if(rotate_r > 255){
			data[3] = 255;
			data[4] = rotate_r - 255;
		}
		if(rotate_r <= 255){
			data[3] = rotate_r;
			data[4] = 0;
		}
// 		MANDO ROTATE_F
		if(rotate_f > 255){
			data[5] = 255;
			data[6] = rotate_f - 255;
		}
		if(rotate_f <= 255){
			data[5] = rotate_f;
			data[6] = 0;
		}
		data[7] = 0;
	}
}
