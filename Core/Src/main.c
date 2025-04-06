#include "main.h"
#include "rccconfig.h"



void TIM2_IRQHandler(void) {
	if (READ_BIT(TIM2->SR, TIM_SR_UIF)) {
		CLEAR_BIT(TIM2->SR, TIM_SR_UIF);  //Сбросим флаг прерывания
		if (!READ_BIT(GPIOC->ODR, GPIO_ODR_ODR13)){
			PortSetHi();
		}
		else{
			PortSetLow();
		}

	}
}

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

	 while (1) {

		}
}
