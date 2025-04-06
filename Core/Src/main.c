#include "main.h"
#include "rccconfig.h"


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

	  int i;

	  for(;;)
	  {
	    PortSetHi();
	    for(i=0; i<0x4000; i++)
	      ;
	    PortSetLow();
	    for(i=0; i<0x4000; i++)
	      ;
	  }
}
