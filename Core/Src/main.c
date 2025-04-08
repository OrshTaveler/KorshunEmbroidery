#include "main.h"
#include "rccconfig.h"
#include "timers.h"




void PortSetHi(void)
{
  GPIOC->BSRR = (1<<13);
}
void PortSetLow(void)
{
  GPIOC->BRR = (1<<13);
}


int main(void){
	 configRcc();
	 configGPIO();
	 configTIM2();
	 configTIM3();
	 while (1) {

	 }
}
