#include "main.h"
#include "rccconfig.h"
#include "timers.h"
#include "GPIO.h"



void PortSetHi(void)
{
	PIN_HIGH(GPIOC,13);
}
void PortSetLow(void)
{
	PIN_LOW(GPIOC,13);
}


int main(void){
	 configRcc();
	 configGPIO();
	 //configTIM2();
	 configTIM3();
	 while (1) {
		 PortSetHi();
		 delayMs(100);
		 PortSetLow();
		 delayMs(100);
	 }
}
