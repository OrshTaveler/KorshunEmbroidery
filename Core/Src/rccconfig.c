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
	// Enable clocking of GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	// Seting CRH to all 0
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
	// Setting CRH 13 to output with max frequency of 10 MHz
	GPIOC->CRH |= (0b01 << GPIO_CRH_MODE13_Pos) | (0b0 << GPIO_CRH_CNF13_Pos);

	GPIOC->BRR = (1<<13);
}

void configTIM2(){
	//Настройка таймера
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Включаем тактирование таймера TIM2

	CLEAR_BIT(TIM2->CR1, TIM_CR1_UDIS);// Включаем генерацию событий Update

	SET_BIT(TIM2->CR1, TIM_CR1_ARPE); // Включаем буферизацию (без понятия что это, включил чтоб было)

	CLEAR_BIT(TIM2->CR1, TIM_CR1_URS);// Генерируем прерывания при переполнении

	SET_BIT(TIM2->CR1, TIM_CR1_ARPE); // Начинаем считать заново когда досчитали

	SET_BIT(TIM2->DIER, TIM_DIER_UIE); // Включаем прерывания по Update

	TIM2->PSC = 800 - 1; // прескалер делит на 8000
	TIM2->ARR = 10000 - 1; // авто-релоуд когда досчитали до 8000
	// Таким образом прерывания по переполнению должны возникать раз в секунду (8МГц / 8000 * 8000 = 1Гц)
	// То есть ШИМ будет тоже с частотой 1Гц
	//Настройка ШИМ


	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_CC1S_Msk, 0b00 << TIM_CCMR1_CC1S_Pos); // Настраиваем первый канал на выход
	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_OC1M_Msk, 0b110 << TIM_CCMR1_OC1M_Pos); // Настраиваем таймер в ШИМ мод 1 т.е. в считающий от 0 до CCR1



	NVIC_EnableIRQ(TIM2_IRQn);
	SET_BIT(TIM2->CR1, TIM_CR1_CEN); // Включаем таймер TIM2

}

