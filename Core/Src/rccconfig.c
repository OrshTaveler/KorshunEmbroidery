#include "rccconfig.h"

int configRcc(){


	  RCC->CR |= (1<<RCC_CR_HSION_Pos); //Запускаем генератор HSI
	  while((RCC->CR & RCC_CR_HSIRDY) == 0);


	  SET_BIT(RCC->CR, RCC_CR_HSEON); //Запустим внешний кварцевый резонатор. Он у нас на 8 MHz.
	  while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0); //Дождемся поднятия флага о готовности

	  SET_BIT(RCC->CR, RCC_CR_CSSON); //Включим CSS

	  //Делители
	  RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);

	  RCC->CFGR &= ~RCC_CFGR_SW;
	  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); //Выберем PLL в качестве System Clock

	  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1); //APB Prescaler /1

	  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2); //010 Two wait states, if 48 MHz < SYSCLK <= 72 MHz
	  SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);

	  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2); //APB1 Prescaler /2, т.к. PCLK1 max 36MHz

	  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);

	  MODIFY_REG(RCC->CFGR, RCC_CFGR_ADCPRE, RCC_CFGR_ADCPRE_DIV6); //ADC Prescaler /6, чтоб было 12MHz, т.к. максимальная частота тут 14 MHz

	  SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC); //В качестве входного сигнала для PLL выберем HSE

	  CLEAR_BIT(RCC->CFGR, RCC_CFGR_PLLXTPRE_HSE); //Никакое предделение перед PLL нам не нужно. Поэтому /1.

	  MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLMULL, RCC_CFGR_PLLMULL9); //т.к. кварц у нас 8Mhz, а нам нужно 72MHz, то в PLL нужно сделать умножение на 9. 8MHz * 9 = 72MHz.

	  CLEAR_BIT(RCC->CFGR, RCC_CFGR_USBPRE); //Для USB 72MHz/1.5 = 48MHz

	  SET_BIT(RCC->CR, RCC_CR_PLLON); //Запустим PLL

	  while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0); //Дожидемся поднятия флага включения PLL
	  return 0;
}

void configGPIO(){
	// Enable clocking of GPIOC and GPIOA and GPIOB
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;


	// Seting CRH to all 0
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
	// Setting CRH 13 to output with max frequency of 10 MHz
	GPIOC->CRH |= (0b01 << GPIO_CRH_MODE13_Pos) | (0b0 << GPIO_CRH_CNF13_Pos);


	// конфигурируем 11 пин регистра B на выход
	GPIOB->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
	GPIOB->CRH |= (0b01 << GPIO_CRH_MODE11_Pos) | (0b00 << GPIO_CRH_CNF11_Pos);

	// конфигурируем 10 пин регистра B на выход
	GPIOB->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
	GPIOB->CRH |= (0b01 << GPIO_CRH_MODE10_Pos) | (0b00 << GPIO_CRH_CNF10_Pos);


}



