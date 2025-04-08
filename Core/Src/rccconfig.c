#include "rccconfig.h"

int configRcc(){


	  RCC->CR |= (1<<RCC_CR_HSION_Pos); //Запускаем генератор HSI

	  //Делители
	  RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);

	  RCC->CFGR &= ~RCC_CFGR_SW;
	  RCC->CFGR |= (0x00<<RCC_CFGR_SW_Pos); //Переключаемся на работу от HSI

	  //Ждем, пока переключимся
	  while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0x00<<RCC_CFGR_SWS_Pos))
	  {
	  }
	  return 0;
}

void configGPIO(){
	// Enable clocking of GPIOC and GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;


	// Seting CRH to all 0
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
	// Setting CRH 13 to output with max frequency of 10 MHz
	GPIOC->CRH |= (0b01 << GPIO_CRH_MODE13_Pos) | (0b0 << GPIO_CRH_CNF13_Pos);

	GPIOC->BRR = (1<<13);
}



